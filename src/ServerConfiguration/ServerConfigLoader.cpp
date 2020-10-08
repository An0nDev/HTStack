#include "ServerConfigLoader.hpp"
#include "../SSL/SSLSetupVars.hpp"

#include <openssl/ssl.h>

#include <fstream>
#include <stdexcept>
#include <optional>
#include <vector>
#include <thread>

namespace HTStack {
    std::string ServerConfigLoader::keyValueSeparator = ": ";
    std::string ServerConfigLoader::sslConfigFieldSeparator = ", ";
    SSLSetupVars ServerConfigLoader::_processSSL (std::string const & ssl_) {
        std::vector <std::string> sslConfigFields;
        std::string sslConfigString (ssl_); // We need to modify so make a copy
        while (true) {
            std::string::size_type fieldSeparatorPosition = sslConfigString.find (sslConfigFieldSeparator);
            if (fieldSeparatorPosition == std::string::npos) {
                sslConfigFields.push_back (sslConfigString);
                break;
            }
            sslConfigFields.push_back (sslConfigString.substr (0, fieldSeparatorPosition));
            sslConfigString = sslConfigString.substr (fieldSeparatorPosition + sslConfigFieldSeparator.size ());
        };
        if (sslConfigFields.size () < 1) {
            throw std::runtime_error ("SSL config needs at least one field (disable in first field to disable)");
        }
        if (sslConfigFields [0] == "disable") {
            return SSLSetupVars ();
        } else if (sslConfigFields [0] != "enable") {
            throw std::runtime_error ("First SSL config field should be either disable or enable");
        }
        if (sslConfigFields.size () != 4) {
            throw std::runtime_error ("Enabled SSL config should have 4 fields (enabled, certChainFilePath, keyFilePath, keyFileType)");
        }
        std::string certChainFilePath = sslConfigFields [1];
        std::string keyFilePath = sslConfigFields [2];
        int keyFileType;
        if (sslConfigFields [3] == "PEM") {
            keyFileType = SSL_FILETYPE_PEM;
        } else if (sslConfigFields [3] == "ASN1") {
            keyFileType = SSL_FILETYPE_ASN1;
        } else {
            throw std::runtime_error ("keyFileType should be PEM or ASN1");
        }
        return SSLSetupVars (
            certChainFilePath,
            keyFilePath,
            keyFileType
        );
    };
    ServerConfiguration ServerConfigLoader::load (std::string const & filePath) {
        std::map <std::string, std::string> map;

        {
            std::ifstream inputStream (filePath, std::ios::in);
            if (!inputStream) {
                throw std::runtime_error ("Unable to open server config path for reading");
            }
            inputStream.exceptions (std::ios_base::badbit);
            while (true) { // Iterating over all key-value pairs
                std::string line;
                std::getline (inputStream, line);
                if (inputStream.eof ()) {
                    break;
                }
                std::string::size_type keyValueSeparatorPosition = line.find (keyValueSeparator);
                if (keyValueSeparatorPosition == std::string::npos) {
                    throw std::runtime_error ("Unable to read server config: missing key-value separator");
                }
                std::string key (line.substr (0, keyValueSeparatorPosition));
                std::string value (line.substr (keyValueSeparatorPosition + keyValueSeparator.size ()));
                map [key] = value;
            }
        }


        #define RETRIEVE_KEY(KEY) std::string KEY ## _ = map.at (#KEY)
        #define RETRIEVE_STR(KEY) std::string KEY = map.at (#KEY)
        #define KEY_TO_INT(KEY) int KEY = std::stoi (KEY ## _);
        #define KEY_TO_BOOL(KEY) \
            bool KEY; \
            if (KEY ## _ == "true") {\
                KEY = true;\
            } else if (KEY ## _ == "false") {\
                KEY = false;\
            } else {\
                throw std::runtime_error ("Couldn't parse KEY as bool");\
            }

        RETRIEVE_STR(appConfigPath);
        RETRIEVE_KEY(autoStart);
        KEY_TO_BOOL(autoStart);
        RETRIEVE_STR(ip);
        RETRIEVE_KEY(port);
        KEY_TO_INT(port);
        RETRIEVE_KEY(backlog);
        KEY_TO_INT(backlog);
        RETRIEVE_KEY(clientThreadPoolSize);
        int clientThreadPoolSize =
        clientThreadPoolSize_ == "auto"
        ? std::thread::hardware_concurrency ()
        : std::stoi (clientThreadPoolSize_);
        RETRIEVE_KEY(maxRecvSize);
        KEY_TO_INT(maxRecvSize);
        RETRIEVE_KEY(sloppySocketRestart);
        KEY_TO_BOOL(sloppySocketRestart);
        RETRIEVE_KEY(streamedResponseBufferSize);
        KEY_TO_INT(streamedResponseBufferSize);
        RETRIEVE_KEY(ssl);
        SSLSetupVars ssl = _processSSL (ssl_);

        return ServerConfiguration (
            // File-based server config only supports using an app config file
            ServerConfiguration::AppSetupType::FILE,
            std::nullopt, // No vector of static configs
            appConfigPath,

            autoStart,
            ip,
            port,
            backlog,
            clientThreadPoolSize,
            maxRecvSize,
            sloppySocketRestart,
            streamedResponseBufferSize,
            ssl
        );
    };
};

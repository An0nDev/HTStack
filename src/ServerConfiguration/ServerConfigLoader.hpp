#include "ServerConfiguration.hpp"

#include "../SSL/SSLSetupVars.hpp"

#include <string>

namespace HTStack {
    class ServerConfigLoader {
        static std::string keyValueSeparator;
        static std::string sslConfigFieldSeparator;
        static SSLSetupVars _processSSL (std::string const & ssl_);
    public:
        static ServerConfiguration load (std::string const & filePath);
    };
};

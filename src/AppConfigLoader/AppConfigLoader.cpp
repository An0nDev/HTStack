#include "AppConfigLoader.hpp"

#include <fstream>
#include <map>

namespace HTStack {
    AppConfigLoader::AppConfigLoader (AppLoader const & appLoader_, std::string const & filePath_)
    : appLoader (appLoader_), filePath (filePath_) {};
    void AppConfigLoader::load () {
        std::ifstream inputStream (filePath, std::ios::in);
        if (!inputStream) {
            throw std::runtime_error ("Unable to open app config path for reading");
        }
        inputStream.exceptions (std::ios_base::failbit | std::ios_base::badbit);
        while (true) { // Iterating over all applications
            if (inputStream.eof ()) { // After reading the last application, we're at the end of the file
                break;
            }
            std::string appNameWithColon;
            std::getline (inputStream, appNameWithColon);
            if (appNameWithColon == "") { // Empty line, presumably as spacing
                continue;
            }
            if (!appNameWithColon.ends_with (":")) {
                throw std::runtime_error ("Unable to read app config: missing colon after app name");
            }
            std::string appName (appNameWithColon.substr (0, appNameWithColon.size () - 1));
            std::string appLocation;
            std::map <std::string, std::string> appSettings;
            bool haveAppLoaded = false;
            bool appLoaded;
            while (true) { // Iterating over all configuration lines for this application
                std::string configLineWithIndent;
                std::getline (inputStream, configLineWithIndent);
                if (inputStream.eof ()) { // Reached end-of-file
                    break;
                }
                if (!configLineWithIndent.starts_with (indent)) { // We unindented
                    break;
                }
                std::string configLine (configLineWithIndent.substr (indent.size ()));
                std::string::size_type keyValueSeparatorPosition (configLine);
                if (keyValueSeparatorPosition == std::string::npos) {
                    throw std::runtime_error ("Unable to read app config: missing key-value separator in app config line");
                }
                std::string configKey (configLine.substr (0, keyValueSeparatorPosition));
                std::string configValue (configLine.substr (keyValueSeparatorPosition + keyValueSeparator.size ()));
                if (configKey == "_location") {
                    appLocation = configValue;
                } else if (configKey == "_loaded") {
                    if (configValue == "true") {
                        appLoaded = true;
                    } else if (configValue == "false") {
                        appLoaded = false;
                    } else {
                        throw std::runtime_error ("Unable to read app config: invalid loaded value for " + appName);
                    }
                    haveAppLoaded = true;
                } else {
                    appSettings [configKey] = configValue;
                }
            }

            if (appLocation == "") {
                throw std::runtime_error ("Unable to read app config: _location for " + appName + " missing or empty");
            }

            if (!haveAppLoaded) {
                throw std::runtime_error ("Unable to read app config: _loaded for " + appName + " missing");
            }

            line.clear ();
        }
    };
    void AppConfigLoader::save () {

    };
    ~AppConfigLoader () {

    }
};

#include "AppConfigLoader.hpp"
#include "../AppLoader/AppContainer.hpp"
#include "../AppLoader/AppLoader.hpp"

#include <fstream>
#include <iostream>
#include <map>

namespace HTStack {
    const std::string AppConfigLoader::keyValueSeparator = ": ";
    const std::string AppConfigLoader::indent = "    ";
    AppConfigLoader::AppConfigLoader (AppLoader & appLoader_, std::string const & filePath_)
    : appLoader (appLoader_), filePath (filePath_) {};
    void AppConfigLoader::load () {
        std::ifstream inputStream (filePath, std::ios::in);
        if (!inputStream) {
            throw std::runtime_error ("Unable to open app config path for reading");
        }
        inputStream.exceptions (std::ios_base::badbit);
        while (true) { // Iterating over all applications
            std::string appNameWithColon;
            std::getline (inputStream, appNameWithColon);
            if (inputStream.eof () || inputStream.bad ()) { // After reading the last application, we're at the end of the file
                break;
            }
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
                if (inputStream.eof () || inputStream.bad ()) { // Reached end-of-file
                    break;
                }
                if (!configLineWithIndent.starts_with (indent)) { // We unindented
                    break;
                }
                std::string configLine (configLineWithIndent.substr (indent.size ()));
                std::string::size_type keyValueSeparatorPosition (configLine.find (keyValueSeparator));
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
            AppContainer* appContainer = new AppContainer (
                appLoader.server,
                appName,
                appLocation,
                appSettings,
                appLoaded
            );
            appLoader.apps.push_back (appContainer);
        }
    };
    void AppConfigLoader::save () {
        std::ostream* outputStream;
        std::ofstream realOutputStream (filePath, std::ios::out);
        bool openedProperly;
        if (!realOutputStream) {
            std::cerr << "Unable to open app config path for writing, dumping to stdout and throwing runtime_error" << std::endl;
            outputStream = &(std::cout);
            openedProperly = false;
        } else { // don't mess with std::cout's exceptions!
            realOutputStream.exceptions (std::ios_base::failbit | std::ios_base::badbit);
            outputStream = &realOutputStream;
            openedProperly = true;
        }
        int appNumber = 1;
        for (AppContainer* app : appLoader.apps) {
            *outputStream << app->name << ":" << std::endl;
            *outputStream << indent << "_location" << keyValueSeparator << app->location << std::endl;
            *outputStream << indent << "_loaded" << keyValueSeparator << (app->isLoaded ? "true" : "false") << std::endl;
            for (std::pair <std::string, std::string> settingPair : app->settings) {
                *outputStream << indent << settingPair.first << keyValueSeparator << settingPair.second << std::endl;
            }
            if (appNumber != appLoader.apps.size ()) *outputStream << std::endl;

            appNumber += 1;
        }
        if (!openedProperly) {
            throw std::runtime_error ("Unable to open app config path for reading");
        }
    };
};

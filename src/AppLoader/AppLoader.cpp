#include "AppLoader.hpp"
#include "../AppConfigLoader/StaticAppConfig.hpp"
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../ExceptionUtils/ExceptionUtils.hpp"
#include "AppContainer.hpp"
#include "../Request/Request.hpp"
#include "../App/App.hpp"
#include <exception>
#include <stdexcept>
#include <filesystem>

#include <iostream> // debug

namespace HTStack {
    AppLoader::AppLoader (Server & server_) : server (server_), appConfigLoader (nullptr) {};
    void AppLoader::start () {
        switch (server.configuration.appSetupType) {
            case ServerConfiguration::AppSetupType::INLINE: {
                for (StaticAppConfig const & appStaticConfig : *(server.configuration.appStaticConfigs)) {
                    AppContainer* appContainer = new AppContainer (server, appStaticConfig.name, appStaticConfig.location, appStaticConfig.settings, appStaticConfig.isLoaded);
                    apps.push_back (appContainer);
                }
                break;
            }
            case ServerConfiguration::AppSetupType::FILE: {
                appConfigLoader = new AppConfigLoader (*this, server.configuration.appConfigPath.value ());
                appConfigLoader->load ();
                appConfigLoader->save (); // In case apps fail to load or modify their own configuration
                break;
            }
        }
    };
    void AppLoader::_setupTypeCheck (std::string const & operation) {
        if (server.configuration.appSetupType == ServerConfiguration::AppSetupType::INLINE) {
            throw std::runtime_error ("Can't " + operation + " with an inline application setup type; consider using a file instead");
        }
    };
    AppContainer* AppLoader::_find (std::string const & appName) {
        for (AppContainer* appContainer : apps) {
            if (appContainer->name == appName) {
                return appContainer;
            }
        }
        throw std::runtime_error ("Could not find app with name " + appName);
    };
    void AppLoader::list () {
        std::lock_guard <std::mutex> guard (appAccessLock);
        std::string list_;
        for (AppContainer* appContainer : apps) {
            list_ += appContainer->name;
            list_ += ", ";
        }
        list_ = list_.substr (0, list_.length () - std::string (", ").length ());
        std::cout << "Apps: " << list_ << std::endl;
    };
    void AppLoader::add (std::string const & appName, std::string const & appLocation) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("add an application");
        // Make a new app; defaults to empty config and unloaded
        AppContainer* appContainer = new AppContainer (server, appName, appLocation, std::map <std::string, std::string> (), false);
        apps.push_back (appContainer);
        appConfigLoader->save ();
    };
    void AppLoader::load (std::string const & appName) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("load an unloaded application");
        AppContainer* appContainer (_find (appName));
        if (appContainer->isLoaded) {
            throw std::runtime_error ("Application is already loaded");
        }
        appContainer->load ();
        appConfigLoader->save ();
    };
    void AppLoader::show (std::string const & appName) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        AppContainer* appContainer (_find (appName));
        std::cout << "Name: " << appContainer->name << std::endl;
        std::cout << "Location: " << appContainer->location << std::endl;
        std::cout << "Is loaded: " << (appContainer->isLoaded ? "true" : "false") << std::endl;
        std::cout << "Settings: " << std::endl;
        for (std::pair <std::string, std::string> configurationPair : appContainer->settings) {
            std::cout << configurationPair.first << ": " << configurationPair.second << std::endl;
        };
    };
    void AppLoader::configure (std::string const & appName, std::string const & key, std::string const & value) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("modify an application's configuration");
        AppContainer* appContainer (_find (appName));
        appContainer->settings [key] = value;
        if (appContainer->isLoaded) {
            appContainer->app->handleSettingsUpdate (key);
        }
        appConfigLoader->save ();
    };
    void AppLoader::unconfigure (std::string const & appName, std::string const & key) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("remove a key from an application's configuration");
        AppContainer* appContainer (_find (appName));
        if (!(appContainer->settings.contains (key))) {
            throw std::runtime_error ("Application's configuration does not contain the key " + key);
        }
        appContainer->settings.erase (key);
        if (appContainer->isLoaded) {
            appContainer->app->handleSettingsRemove (key);
        }
        appConfigLoader->save ();
    };
    void AppLoader::unload (std::string const & appName) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("unload a loaded application");
        AppContainer* appContainer (_find (appName));
        if (!appContainer->isLoaded) {
            throw std::runtime_error ("Application is already unloaded");
        }
        appContainer->unload ();
        appConfigLoader->save ();
    };
    void AppLoader::reload (std::string const & appName) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("reload an application");
        AppContainer* appContainer (_find (appName));
        if (appContainer->isLoaded) {
            appContainer->unload ();
        }
        appContainer->load ();
        appConfigLoader->save ();
    };
    void AppLoader::remove (std::string const & appName) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        _setupTypeCheck ("remove an application");
        int appIndex = 0;
        std::vector <AppContainer*>::iterator removeTarget (apps.begin ());
        for (AppContainer* appContainer : apps) {
            if (appContainer->name == appName) {
                delete appContainer;
                apps.erase (removeTarget);
                appConfigLoader->save ();
                return;
            }
            removeTarget += 1;
        }
        throw std::runtime_error ("Could not find app with name " + appName);
    };
    void AppLoader::handleRequest (Request & request) {
        std::lock_guard <std::mutex> guard (appAccessLock);
        for (AppContainer* appContainer : apps) {
            if (!appContainer->isLoaded) {
                continue;
            }
            bool changingDirs = appContainer->locationDirectory.has_value ();
            std::filesystem::path oldPath;
            if (changingDirs) {
                oldPath = std::filesystem::current_path ();
                std::filesystem::current_path (appContainer->locationDirectory.value ());
            }
            App* app = appContainer->app;
            app->handleRequest (request);
            if (changingDirs) std::filesystem::current_path (oldPath);
            if (request.complete) {
                return;
            }
        };
    };
    void AppLoader::shutdown () {
        if (appConfigLoader != nullptr) {
            delete appConfigLoader;
        }
        for (AppContainer* appContainer : apps) {
            delete appContainer;
        }
        apps.clear ();
    };
    AppLoader::~AppLoader () {
        for (AppContainer* appContainer : apps) {
            if (appContainer->isLoaded) {
                try {
                    appContainer->unload ();
                } catch (const std::runtime_error & e) {
                    ExceptionUtils::safeThrow (e);
                }
            }
            delete appContainer;
        }
    }
};

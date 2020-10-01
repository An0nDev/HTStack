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

#include <iostream> // debug

namespace HTStack {
    AppLoader::AppLoader (Server & server_) : server (server_), appConfigLoader (nullptr) {};
    void AppLoader::start () {
        std::cout << "AppLoader::start called" << std::endl;
        switch (server.configuration.appSetupType) {
            case ServerConfiguration::AppSetupType::INLINE: {
                std::cout << "Inline setup" << std::endl;
                for (StaticAppConfig const & appStaticConfig : *(server.configuration.appStaticConfigs)) {
                    std::cout << "Constructing appContainer for " << appStaticConfig.name << std::endl;
                    AppContainer* appContainer = new AppContainer (server, appStaticConfig.name, appStaticConfig.location, appStaticConfig.settings, appStaticConfig.isLoaded);
                    apps.push_back (appContainer);
                }
                std::cout << "Done with inline setup" << std::endl;
                break;
            }
            case ServerConfiguration::AppSetupType::FILE: {
                appConfigLoader = new AppConfigLoader (*this, server.configuration.appConfigPath.value ());
                appConfigLoader->load ();
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
    void AppLoader::add (std::string const & appName, std::string const & appLocation) {
        _setupTypeCheck ("add an application");
        // Make a new app; defaults to empty config and unloaded
        AppContainer* appContainer = new AppContainer (server, appName, appLocation, std::map <std::string, std::string> (), false);
        apps.push_back (appContainer);
        appConfigLoader->save ();
    };
    void AppLoader::load (std::string const & appName) {
        _setupTypeCheck ("load an unloaded application");
        AppContainer* appContainer (_find (appName));
        if (appContainer->isLoaded) {
            throw std::runtime_error ("Application is already loaded");
        }
        appContainer->load ();
        appConfigLoader->save ();
    };
    void AppLoader::configure (std::string const & appName, std::string const & key, std::string const & value) {
        AppContainer* appContainer (_find (appName));
        appContainer->settings [key] = value;
        appConfigLoader->save ();
    };
    void AppLoader::unload (std::string const & appName) {
        _setupTypeCheck ("unload a loaded application");
        AppContainer* appContainer (_find (appName));
        if (!appContainer->isLoaded) {
            throw std::runtime_error ("Application is already unloaded");
        }
        appContainer->unload ();
        appConfigLoader->save ();
    };
    void AppLoader::remove (std::string const & appName) {
        _setupTypeCheck ("remove an application");
        int appIndex = 0;
        std::vector <AppContainer*>::iterator removeTarget (apps.begin ());
        for (AppContainer* appContainer : apps) {
            if (appContainer->name == appName) {
                delete appContainer;
                apps.erase (removeTarget);
                appConfigLoader->save ();
                break;
            }
            removeTarget += 1;
        }
        throw std::runtime_error ("Could not find app with name " + appName);
    };
    void AppLoader::handleRequest (Request & request) {
        for (AppContainer* appContainer : apps) {
            if (!appContainer->isLoaded) {
                continue;
            }
            App* app = appContainer->app;
            app->onRequest (request);
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

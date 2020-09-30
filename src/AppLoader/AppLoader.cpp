#include "AppLoader.hpp"
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../ExceptionUtils/ExceptionUtils.hpp"
#include "AppContainer.hpp"
#include "../Request/Request.hpp"
#include "../App/App.hpp"
#include <exception>
#include <stdexcept>

namespace HTStack {
    AppLoader::AppLoader (Server & server_) : server (server_), appConfigLoader (std::nullopt) {};
    void AppLoader::start () {
        switch (server.configuration.appSetupType) {
            case ServerConfiguration::AppSetupType::INLINE: {
                for (std::string const & appLocation : server.configuration.appLocations) {
                    AppContainer* appContainer = new AppContainer (appLocation, appLocation, std::map <std::string, std::string> (), true);
                    apps.push_back (appContainer);
                    appContainer->load (server);
                }
                break;
            }
            case ServerConfiguration::AppSetupType::FILE: {
                appConfigLoader = std::optional <AppConfigLoader> (std::in_place_t, *this, server.configuration.appConfigPath.value ());
                appConfigLoader->load ();
                break;
            }
        }
    };
    AppContainer* AppLoader::_find (std::string const & appName) {
        for (AppContainer* appContainer : apps) {
            if (appContainer->name == appName) {
                return appContainer;
            }
        }
        throw std::runtime_error ("Could not find app with name " + appName);
    }
    void AppLoader::add (std::string const & appName, std::string const & appLocation)
    void AppLoader::load (std::string const & appName) {
        AppContainer* appContainer (_find (appName));
        if (!appContainer->isLoaded) {
            appContainer->load (server);
            appConfigLoader->save ();
        }
    };
    void AppLoader::configure (std::string const & key, std::string const & value) {
        AppContainer* appContainer (_find (appName));
        appContainer->settings [key] = value;
        appConfigLoader->save ();
    };
    void AppLoader::unload (std::string const & appName) {
        if (appContainer->isLoaded) {
            appContainer->load (server);
        }
        appConfigLoader->save ();
    };
    void AppLoader::remove (std::string const & appName, std::string const & )
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

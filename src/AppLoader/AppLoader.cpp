#include "AppLoader.hpp"
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "AppContainer.hpp"
#include "../Request/Request.hpp"
#include "../App/App.hpp"
#include <exception>

namespace HTStack {
    AppLoader::AppLoader (Server & server_) : server (server_) {
        for (std::string const & appLocation : server.configuration.appLocations) {
            AppContainer* appContainer = new AppContainer (appLocation);
            apps.push_back (appContainer);
        }
    };
    void AppLoader::loadAll () {
        for (AppContainer* appContainer : apps) {
            appContainer->load (server);
        }
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
    void AppLoader::unloadAll () {
        for (AppContainer* appContainer : apps) {
            appContainer->unload ();
        };
    };
    AppLoader::~AppLoader () {
        for (AppContainer* appContainer : apps) {
            if (appContainer->isLoaded) {
                try {
                    appContainer->unload ();
                } catch (const std::runtime_error & e) {
                    if (std::uncaught_exceptions () > 0) throw e;
                }
            }
            delete appContainer;
        }
    }
};

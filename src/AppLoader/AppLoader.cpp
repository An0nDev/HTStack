#include "AppLoader.hpp"
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "AppContainer.hpp"
#include "../Request/Request.hpp"
#include "../App/App.hpp"

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
                appContainer->unload ();
            }
            delete appContainer;
        }
    }
};

#pragma once

#include "../AppConfigLoader/AppConfigLoader.hpp"

#include <vector>
#include <string>
#include <stdexcept>

namespace HTStack {
    class Server;
    class AppContainer;
    class Request;
    class AppLoader {
        Server & server;
        std::vector <AppContainer*> apps;
        std::optional <AppConfigLoader> appConfigLoader;

        _find (std::string const & appName);
    public:
        explicit AppLoader (Server & server_);
        void loadAll ();
        void handleRequest (Request & request);
        void unloadAll ();
        ~AppLoader ();
    };
};

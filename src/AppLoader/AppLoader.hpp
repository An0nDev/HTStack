#pragma once
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
    public:
        AppLoader (Server & server_);
        void loadAll ();
        void handleRequest (Request & request);
        void unloadAll ();
        ~AppLoader () noexcept (false);
    };
};

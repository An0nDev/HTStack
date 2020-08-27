#pragma once
#include <vector>
#include <string>
#include <stdexcept>

namespace HTStack {
    class Server;
    class AppContainer;
    class AppLoader {
        Server & server;
        std::vector <AppContainer*> apps;
    public:
        AppLoader (Server & server_);
        void loadAll ();
        void unloadAll ();
        ~AppLoader () throw (std::logic_error);
    };
};

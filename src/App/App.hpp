#pragma once
#include "../AppLoader/AppContainer.hpp"
#include "../Server/Server.hpp"

namespace HTStack {
    class Server;
    class Request;
    class App {
    protected:
        Server & server;
    public:
        App (Server & server_);
        virtual void handleRequest (Request & request) = 0;
        virtual ~App () {};
    };
};

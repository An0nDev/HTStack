#pragma once
#include "../AppLoader/AppContainer.hpp"
#include "../Server/Server.hpp"

class Server;
class Request;

namespace HTStack {
    class App {
        Server* server = nullptr; // Pointer to the server containing this app (initialized before on* functions called)
        friend void AppContainer::load (Server & server);
    public:
        virtual void onLoad () = 0;
        virtual void onRequest (Request & request) = 0;
        virtual void onUnload () = 0;
        virtual ~App () {};
    };
};

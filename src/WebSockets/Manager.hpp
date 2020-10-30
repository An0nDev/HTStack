#pragma once

#include "../Request/Request.hpp"
#include "../ThreadingUtils/Event.hpp"
#include "ClientThread.hpp"
#include <mutex>

namespace HTStack {
    class Server;
};

namespace HTStack::WebSockets {
    class Manager {
    private:
        Server & server;
        std::mutex threadsLock;
        std::vector <ClientThread*> threads;

        Event cleanupOrShutdownEvent;
        std::mutex cleaningUpLock;
        std::mutex shuttingDownLock;
        bool shuttingDown;
        std::thread* cleanupThread;
        void cleanupFunc ();
        friend ClientThread;
    public:
        Manager (Server & server_);
        void start ();
        void handle (WebSocket* const & webSocket);
        void shutdown ();
    };
};

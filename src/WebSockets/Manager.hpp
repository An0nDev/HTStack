#pragma once

#include "../Server/Server.hpp"
#include "../Request/Request.hpp"
#include "../ThreadingUtils/Event.hpp"
#include <mutex>

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
    public:
        Manager (Server & server_);
        void start ();
        void handle (Request & request);
        void shutdown ();
    };
};

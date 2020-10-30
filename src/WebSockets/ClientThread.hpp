#pragma once

#include "WebSocket.hpp"

#include <thread>
#include <mutex>

namespace HTStack::WebSockets {
    class Manager;
    class ClientThread {
    private:
        Manager & manager;

        std::thread thread;
        void func (WebSocket* const & webSocket);

        void run_ (WebSocket* const & webSocket);

        void cleanup_ ();
        std::mutex finishedLock;
        bool finished = false;
        friend Manager;
    public:
        ClientThread (Manager & manager_, WebSocket* const & webSocket);
        ~ClientThread ();
    };
};

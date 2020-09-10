#pragma once

#include <thread>
#include <sys/types.h>
#include <netinet/ip.h>
#include <system_error>
#include <mutex>
#include "../SocketClientManager/SocketClientManager.hpp"

namespace HTStack {
    class Server;
    class SocketManager {
    public:
        Server & server;
        bool isRunning = false;
        explicit SocketManager (Server & server_);
        void start ();
        void shutdown ();
        ~SocketManager ();
    private:
        std::thread* runThread;

        sockaddr_in serverAddress;
        int serverSocket;

        void setup_ ();
        void run_ ();
        void cleanup_ ();

        SocketClientManager clientManager;
    };
};

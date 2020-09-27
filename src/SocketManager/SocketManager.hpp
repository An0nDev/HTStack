#pragma once

#include "../SocketClientManager/SocketClientManager.hpp"

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <thread>
#include <system_error>
#include <mutex>

#include <sys/types.h>
#include <netinet/ip.h>

namespace HTStack {
    class Server;
    class SocketManager {
    public:
        Server & server;
        bool isRunning;
        explicit SocketManager (Server & server_);
        void start ();
        void shutdown ();
        ~SocketManager ();
    private:
        std::thread* runThread;

        sockaddr_in serverAddress;
        int serverSocket;

        SSL_CTX* sslServer; // Only set if SSL is enabled using ServerConfiguration

        void setup_ ();
        void run_ ();
        void cleanup_ ();

        SocketClientManager clientManager;
    };
};

#pragma once

#include <netinet/ip.h>
#include <vector>
#include <thread>
#include <mutex>

namespace HTStack {
    class Server;
    class SocketClientManager;
    class SocketClientManager {
    private:
        std::vector <std::thread*> clientThreadList;
        // Forces calls to create or waitForAll to wait for any other calls to create or waitForAll to complete first
        // (create is called from SocketManager::runThread and waitForAll is called when SocketManager::shutdown is called)
        // This guarantees thread safety for the clientThreadList object
        std::mutex clientThreadListLock;

        void clientFunc (int const & clientSocket, sockaddr_in const & clientAddress);
    public:
        SocketClientManager (Server & server_);

        Server & server;

        void create (int const & clientSocket, sockaddr_in const & clientAddress);
        // NOTE: waitForAll does *not* disallow future calls to create;
        // make sure to disable the mechanism that calls create before calling waitForAll!
        void waitForAll ();
    };
};

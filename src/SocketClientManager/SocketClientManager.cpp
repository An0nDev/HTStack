#include "SocketClientManager.hpp"
#include "../Server/Server.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../Request/Request.hpp"
#include <unistd.h>

namespace HTStack {
    SocketClientManager::SocketClientManager (Server & server_)
    : server (server_) {};
    void SocketClientManager::clientFunc (int const & clientSocket, sockaddr_in const & clientAddress) {
        Request request = server.requestReader.readFrom (clientSocket, clientAddress);
        server.appLoader.handleRequest (request);

        int shutdownReturnValue = shutdown (clientSocket, SHUT_RDWR);
        SocketManager::system_error_check__ ("shutdown ()", shutdownReturnValue);

        int closeReturnValue = close (clientSocket);
        SocketManager::system_error_check__ ("close ()", closeReturnValue);
    };

    void SocketClientManager::create (int const & clientSocket, sockaddr_in const & clientAddress) {
        clientThreadListLock.unlock ();

        std::thread* newClientThread = new std::thread (&SocketClientManager::clientFunc, this, clientSocket, clientAddress);

        clientThreadListLock.lock ();
    };

    void SocketClientManager::waitForAll () {
        clientThreadListLock.unlock ();

        for (std::thread* clientThread : clientThreadList) {
            clientThread->join ();
            delete clientThread;
        };
        clientThreadList.clear ();

        clientThreadListLock.lock ();
    };
};

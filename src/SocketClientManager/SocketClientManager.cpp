#include "SocketClientManager.hpp"
#include "../Server/Server.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../Request/Request.hpp"
#include <unistd.h>
#include <optional>
#include <iostream>

namespace HTStack {
    SocketClientManager::SocketClientManager (Server & server_)
    : server (server_) {};
    void SocketClientManager::clientFunc (int const & clientSocket, sockaddr_in const & clientAddress) {
        std::optional <Request> request = server.requestReader.readFrom (clientSocket, clientAddress);
        if (request.has_value ()) {
            server.appLoader.handleRequest (request.value ());
        }

        int shutdownReturnValue = shutdown (clientSocket, SHUT_RDWR);
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        int closeReturnValue = close (clientSocket);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
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

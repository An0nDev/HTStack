#include "SocketManager.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include "../CInteropUtils/CInteropUtils.hpp"
#include <errno.h>
#include "../Server/Server.hpp"
#include "../SocketClientManager/SocketClientManager.hpp"

namespace HTStack {
    SocketManager::SocketManager (Server & server_)
    : server (server_), clientManager (server) {
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
    };
    void SocketManager::start () {
        setup_ ();
        runThread = new std::thread (&SocketManager::run_, this);
        // Socket initialization goes here
        isRunning = true;
    };
    void SocketManager::shutdown () {
        std::cout << "Calling cleanup_" << std::endl;
        cleanup_ ();
        isRunning = false;
        std::cout << "Joining run thread" << std::endl;
        runThread->join ();
        delete runThread;
        std::cout << "Done" << std::endl;
        // Socket deconstruction goes here
    };
    void SocketManager::setup_ () {
        serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
        CInteropUtils::systemErrorCheck ("socket ()", serverSocket);

        if (server.configuration.sloppySocketRestart) {
            int reuseAddrValue = 1; // non-zero value for enable since it's a boolean
            int setsockoptReturnValue = setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddrValue, sizeof (reuseAddrValue));
            CInteropUtils::systemErrorCheck ("setsockopt ()", setsockoptReturnValue);
        }

        serverAddress.sin_port = htons (server.configuration.port);
        unsigned long serverAddressSize = sizeof (serverAddress);
        int clientSocket = bind (serverSocket, (sockaddr*) &serverAddress, serverAddressSize);
        CInteropUtils::systemErrorCheck ("bind ()", clientSocket);

        int listenReturnValue = listen (serverSocket, server.configuration.backlog);
        CInteropUtils::systemErrorCheck ("listen ()", listenReturnValue);
    };
    void SocketManager::run_ () {
        while (true) {
            sockaddr_in clientAddress;
            unsigned long clientAddressSize = sizeof (clientAddress);

            int clientSocket = accept (serverSocket, (sockaddr*) &clientAddress, (socklen_t*) &clientAddressSize);
            if (clientSocket < 0 && errno == EINVAL) {
                // Broken from cleanup_
                break;
            }
            CInteropUtils::systemErrorCheck ("accept ()", clientSocket);

            std::cout << "Got client socket! File descriptor: " << clientSocket << std::endl;
            clientManager.create (clientSocket, clientAddress);
        }
    };
    void SocketManager::cleanup_() {
        int shutdownReturnValue = ::shutdown (serverSocket, SHUT_RD); // causes accept to fail with EINVAL; see https://stackoverflow.com/a/19239058/5037905
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        clientManager.waitForAll ();

        int closeReturnValue = close (serverSocket);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
    };
    SocketManager::~SocketManager () {
        if (isRunning) shutdown ();
    };
};

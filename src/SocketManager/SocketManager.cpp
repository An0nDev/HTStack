#include "SocketManager.hpp"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "../Server/Server.hpp"
#include "../SocketClientManager/SocketClientManager.hpp"

namespace HTStack {
    SocketManager::SocketManager (Server & server_)
    : server (server_), clientManager (server) {
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
    };
    void SocketManager::system_error_check__ (std::string const & systemFunctionName, int & systemFunctionReturnValue) throw (std::system_error) {
        if (systemFunctionReturnValue < 0) {
            throw std::system_error (std::error_code (errno, std::system_category ()), systemFunctionName);
        }
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
    void SocketManager::setup_ () throw (std::system_error) {
        serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
        system_error_check__ ("socket ()", serverSocket);

        serverAddress.sin_port = htons (server.configuration.port);
        unsigned long serverAddressSize = sizeof (serverAddress);
        int clientSocket = bind (serverSocket, (sockaddr*) &serverAddress, serverAddressSize);
        system_error_check__ ("bind ()", clientSocket);

        int listenReturnValue = listen (serverSocket, server.configuration.backlog);
        system_error_check__ ("listen ()", listenReturnValue);
    };
    void SocketManager::run_ () throw (std::system_error) {
        while (true) {
            sockaddr_in clientAddress;
            unsigned long clientAddressSize = sizeof (clientAddress);

            int clientSocket = accept (serverSocket, (sockaddr*) &clientAddress, (socklen_t*) &clientAddressSize);
            if (clientSocket < 0 && errno == EINVAL) {
                // Broken from cleanup_
                break;
            }
            system_error_check__ ("accept ()", clientSocket);

            std::cout << "Got client socket! File descriptor: " << clientSocket << std::endl;
            clientManager.create (clientSocket, clientAddress);
        }
    };
    void SocketManager::cleanup_() throw (std::system_error) {
        int shutdownReturnValue = ::shutdown (serverSocket, SHUT_RD); // causes accept to fail with EINVAL; see https://stackoverflow.com/a/19239058/5037905
        system_error_check__ ("shutdown ()", shutdownReturnValue);

        clientManager.waitForAll ();

        int closeReturnValue = close (serverSocket);
        system_error_check__ ("close ()", closeReturnValue);
    };
    SocketManager::~SocketManager () {
        if (isRunning) shutdown ();
    };
};

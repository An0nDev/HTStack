#include "SocketManager.hpp"
#include <stdexcept>
#include <iostream>

namespace HTStack {
    SocketManager::SocketManager (Server & server_)
    : server (server_) {};
    void SocketManager::start () {
        runThread = new std::thread (SocketManagerRun_, this);
        // Socket initialization goes here
        isRunning = true;
    };
    void SocketManager::shutdown () {
        isRunning = false;
        runThread->join ();
        delete runThread;
        // Socket deconstruction goes here
    };
    void SocketManager::run_ () {
        std::cout << "Run thread is running..." << std::endl;
    };
    SocketManager::~SocketManager () throw (std::logic_error) {
        if (isRunning) {
            throw std::logic_error ("SocketManager::isRunning was true on destructor call; call shutdown () first to confirm this behavior");
        }
    };

    void SocketManagerRun_ (SocketManager* socketManager) { // argument to std::thread constructor cannot be a member function
        socketManager->run_ ();
    };
};

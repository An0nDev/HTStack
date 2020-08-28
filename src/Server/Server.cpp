#include "Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include <iostream>
#include <thread>

namespace HTStack {
    Server::Server (ServerConfiguration const & configuration_)
    : configuration (configuration_), appLoader (*this), requestReader (*this), socketManager (*this) {};
    void Server::start () {
        if (isRunning) return;
        ensureAsyncShutdownCompleted ();
        std::cout << "Server starting up" << std::endl;
        if (configuration.autoStart) {
            appLoader.loadAll ();
        }
        socketManager.start ();
        std::cout << "Server started up" << std::endl;
        isRunning = true;
    };
    void Server::shutdown () {
        if (!isRunning) return;
        ensureAsyncShutdownCompleted ();
        isRunning = false;
        std::cout << "Server shutting down" << std::endl;
        socketManager.shutdown ();
        appLoader.unloadAll ();
        std::cout << "Server shut down" << std::endl;
    };
    void Server::shutdownAsync () {
        ensureAsyncShutdownCompleted ();
        wasShutdownWithAsync = true;
        shutdownThread = new std::thread (&Server::shutdown, this);
    };
    void Server::ensureAsyncShutdownCompleted () {
        if (wasShutdownWithAsync) {
            shutdownThread->join ();
            delete shutdownThread;
            shutdownThread = nullptr;
            wasShutdownWithAsync = false;
        }
    };
    Server::~Server () {
        ensureAsyncShutdownCompleted ();
        if (isRunning) shutdown ();
    };
};

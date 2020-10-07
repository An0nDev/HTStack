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
        appLoader.start ();
        socketManager.start ();
        isRunning = true;
    };
    void Server::shutdown () {
        if (!isRunning) return;
        ensureAsyncShutdownCompleted ();
        isRunning = false;
        socketManager.shutdown ();
        appLoader.shutdown ();
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

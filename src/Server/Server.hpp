#pragma once

#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../AppLoader/AppLoader.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../SocketManager/SocketManager.hpp"
#include "../WebSockets/Checker.hpp"

namespace HTStack {
    class ServerConfiguration;
    class Server {
        // Used to ensure operations don't take place before shutdownAsync completes
        bool wasShutdownWithAsync = false;
        std::thread* shutdownThread = nullptr;
        void ensureAsyncShutdownCompleted ();
    public:
        bool isRunning = false;
        ServerConfiguration configuration;
        AppLoader appLoader;
        RequestReader requestReader;
        SocketManager socketManager;
        WebSockets::Checker webSocketChecker;
        explicit Server (ServerConfiguration const & configuration_);
        void start ();
        void shutdown ();

        void shutdownAsync ();

        ~Server ();
    };
};

#pragma once

#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../AppLoader/AppLoader.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../SocketManager/SocketManager.hpp"

namespace HTStack {
    class ServerConfiguration;
    class Server {
    public:
        ServerConfiguration const & configuration;
        AppLoader appLoader;
        RequestReader requestReader;
        SocketManager socketManager;
        Server (ServerConfiguration const & configuration_);
        ~Server ();
    };
};

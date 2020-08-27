#include "Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include <iostream>

namespace HTStack {
    Server::Server (ServerConfiguration const & configuration_)
    : configuration (configuration_), appLoader (*this), requestReader (*this), socketManager (*this) {
        std::cout << "Server starting up" << std::endl;
        if (configuration.autoStart) {
            appLoader.loadAll ();
        }
        socketManager.start ();
        std::cout << "Server started up" << std::endl;
    };
    Server::~Server () {
        std::cout << "Server shutting down" << std::endl;
        socketManager.shutdown ();
        appLoader.unloadAll ();
        std::cout << "Server shut down" << std::endl;
    };
};

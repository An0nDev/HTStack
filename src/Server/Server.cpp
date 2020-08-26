#include "Server.hpp"

namespace HTStack {
    Server::Server (ServerConfiguration const & configuration_) : configuration (configuration_) : appLoader (*this) {
        if (configuration.autoStart) {
            appLoader.loadAll ();
        }
    };
};

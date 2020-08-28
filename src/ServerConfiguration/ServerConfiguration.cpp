#include "ServerConfiguration.hpp"

namespace HTStack {
    ServerConfiguration::ServerConfiguration (
        std::vector <std::string> const & appLocations_,
        bool const & autoStart_,
        int const & port_, int const & backlog_
    ) : appLocations (appLocations_),
    autoStart (autoStart_),
    port (port_),
    backlog (backlog_) {};
};

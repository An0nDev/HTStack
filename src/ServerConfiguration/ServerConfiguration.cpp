#include "ServerConfiguration.hpp"
#include <iostream>

namespace HTStack {
    ServerConfiguration::ServerConfiguration (
        std::vector <std::string> const & appLocations_,
        bool const & autoStart_,
        int const & port_,
        int const & backlog_,
        int const & clientThreadPoolSize_,
        int const & maxRecvSize_,
        bool const & sloppySocketRestart_,
        int const & streamedResponseBufferSize_
    ) : appLocations (appLocations_),
    autoStart (autoStart_),
    port (port_),
    backlog (backlog_),
    clientThreadPoolSize (clientThreadPoolSize_),
    maxRecvSize (maxRecvSize_),
    sloppySocketRestart (sloppySocketRestart_),
    streamedResponseBufferSize (streamedResponseBufferSize_) {};
};

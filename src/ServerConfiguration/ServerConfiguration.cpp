#include "ServerConfiguration.hpp"
#include <iostream>

namespace HTStack {
    ServerConfiguration::ServerConfiguration (
        ServerConfiguration::AppSetupType appSetupType_,
        std::optional <std::vector <StaticAppConfig>> const & appStaticConfigs_,
        std::optional <std::string> const & appConfigPath_,
        bool const & autoStart_,
        std::string const & ip_,
        int const & port_,
        int const & backlog_,
        int const & clientThreadPoolSize_,
        int const & maxRecvSize_,
        bool const & sloppySocketRestart_,
        int const & streamedResponseBufferSize_,
        SSLSetupVars const & ssl_
    ) : appSetupType (appSetupType_),
    appStaticConfigs (appStaticConfigs_),
    appConfigPath (appConfigPath_),
    autoStart (autoStart_),
    ip (ip_),
    port (port_),
    backlog (backlog_),
    clientThreadPoolSize (clientThreadPoolSize_),
    maxRecvSize (maxRecvSize_),
    sloppySocketRestart (sloppySocketRestart_),
    streamedResponseBufferSize (streamedResponseBufferSize_),
    ssl (ssl_) {};
};

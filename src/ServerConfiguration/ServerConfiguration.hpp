#pragma once
#include <vector>
#include <string>
#include "../SSL/SSLSetupVars.hpp"

namespace HTStack {
    class ServerConfiguration {
    public:
        std::vector <std::string> appLocations;
        bool autoStart;
        std::string ip;
        int port;
        int backlog;
        int clientThreadPoolSize;
        int maxRecvSize;
        bool sloppySocketRestart;
        int streamedResponseBufferSize;
        SSLSetupVars ssl;
        ServerConfiguration (
            std::vector <std::string> const & appLocations_,
            bool const & autoStart_,
            std::string const & ip_,
            int const & port_,
            int const & backlog_,
            int const & clientThreadPoolSize_,
            int const & maxRecvSize_,
            bool const & sloppySocketRestart_,
            int const & streamedResponseBufferSize_,
            SSLSetupVars const & ssl_
        );
    };
};

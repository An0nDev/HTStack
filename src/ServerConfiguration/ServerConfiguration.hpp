#pragma once
#include <vector>
#include <string>

namespace HTStack {
    class ServerConfiguration {
    public:
        std::vector <std::string> appLocations;
        bool autoStart;
        int port;
        int backlog;
        int maxRecvSize;
        bool sloppySocketRestart;
        ServerConfiguration (
            std::vector <std::string> const & appLocations_,
            bool const & autoStart_,
            int const & port_,
            int const & backlog_,
            int const & maxRecvSize_,
            bool const & sloppySocketRestart_
        );
    };
};

#pragma once
#include <vector>
#include <string>

namespace HTStack {
    class ServerConfiguration {
    public:
        std::vector <std::string> appLocations;
        bool autoStart;
        ServerConfiguration (std::vector <std::string> const & appLocations_, bool const & autoStart_);
    };
}

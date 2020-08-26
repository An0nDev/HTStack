#include "ServerConfiguration.hpp"

namespace HTStack {
    ServerConfiguration::ServerConfiguration (std::vector <std::string> const & appLocations_, bool const & autoStart_)
    : appLocations (appLocations_) : autoStart (autoStart_) {}
};

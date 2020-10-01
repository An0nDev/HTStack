#include "StaticAppConfig.hpp"

namespace HTStack {
    StaticAppConfig::StaticAppConfig (
        std::string const & name_,
        std::string const & location_,
        std::map <std::string, std::string> const & settings_,
        bool const & isLoaded_
    ) : name (name_), location (location_), settings (settings_), isLoaded (isLoaded_) {};
};

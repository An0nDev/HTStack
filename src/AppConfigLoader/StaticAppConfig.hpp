#include <string>
#include <map>

namespace HTStack {
    class StaticAppConfig {
        const std::string name;
        const std::string location;
        const std::map <std::string, std::string> settings;
        const bool isLoaded;

        StaticAppConfig (
            std::string const & name_,
            std::string const & location
        )
    };
};

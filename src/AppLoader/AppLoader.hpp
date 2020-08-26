#include <vector>

namespace HTStack {
    class AppLoader {
        Server const & server;
        AppLoader (Server const & server_);
        static std::vector <HTStack::App> loadApps (std::vector <std::string const &> const & appLocations);
    };
};

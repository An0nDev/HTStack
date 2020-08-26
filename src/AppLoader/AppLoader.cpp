#include <iostream>
#include "AppLoader.hpp"

namespace HTStack {
    AppLoader::AppLoader (Server const & server_) : server (server_) {}
    AppLoader::loadAll () {
        for (std::string const & appLocation : server.appLocations) {
            std::cout << "Loading app at " << appLocation << std::endl;
        }
    };
};

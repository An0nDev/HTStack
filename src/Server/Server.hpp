#include "../ServerConfiguration/ServerConfiguration.hpp"

namespace HTStack {
    class Server {
        ServerConfiguration const & configuration;
        AppLoader appLoader;
        Server (ServerConfiguration const & configuration_);
    };
};

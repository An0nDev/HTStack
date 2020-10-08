#include "HTStackServer.hpp"

#include "../src/ServerConfiguration/ServerConfigLoader.hpp"
#include "../src/ServerConfiguration/ServerConfiguration.hpp"
#include "../src/Server/Server.hpp"
#include "../src/ServerTerminalWrapper/ServerTerminalWrapper.hpp"

#include <stdexcept>

int main (int argc, char* argv []) {
    if (argc != 2) {
        throw std::runtime_error ("HTStackServer needs exactly one argument, the path to the server config file.");
    }
    HTStack::ServerConfiguration serverConfiguration = HTStack::ServerConfigLoader::load (argv [1]);
    HTStack::Server server (serverConfiguration);
    server.start ();
    HTStack::ServerTerminalWrapper serverTerminalWrapper (server);
    serverTerminalWrapper.prompt (); // TODO handle Ctrl+C properly
    server.shutdown ();
};

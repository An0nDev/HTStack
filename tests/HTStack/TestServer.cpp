#include "TestServer.hpp"
#include <string>

int main () {
    std::string appLocation ("TestApp.so");
    std::vector <std::string> appLocations;
    appLocations.push_back (appLocation);
    HTStack::ServerConfiguration serverConfiguration (
        appLocations,
        true // autoStart
    );
    HTStack::Server server (serverConfiguration);
}

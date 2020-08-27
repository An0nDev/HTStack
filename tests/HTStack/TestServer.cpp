#include "TestServer.hpp"
#include "../../src/ServerConfiguration/ServerConfiguration.hpp"
#include "../../src/Server/Server.hpp"
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>

int main () {
    std::string appLocation ("TestApp.so");
    std::vector <std::string> appLocations;
    appLocations.push_back (appLocation);
    HTStack::ServerConfiguration serverConfiguration (
        appLocations,
        true // autoStart
    );
    HTStack::Server server (serverConfiguration);
    std::cout << "Waiting for signal..." << std::endl;
    pause ();
    std::cout << "Signal received." << std::endl;
};

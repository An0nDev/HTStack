#include "TestServer.hpp"
#include "../../src/ServerConfiguration/ServerConfiguration.hpp"
#include "../../src/Server/Server.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

int main () {
    std::string appLocation ("TestApp.so");
    std::vector <std::string> appLocations;
    appLocations.push_back (appLocation);
    HTStack::ServerConfiguration serverConfiguration (
        appLocations,
        true, // autoStart
        8080, // port
        1024, // backlog
        65535, // maxRecvSize
        true // sloppySocketRestart
    );
    {
        HTStack::Server server (serverConfiguration);
        server.start ();
        std::cout << "Waiting for 5 seconds..." << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds (5));
        std::cout << "Slept for 5 seconds, shutting down server." << std::endl;
        server.shutdown ();
    }
};

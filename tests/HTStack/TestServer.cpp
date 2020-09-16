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
        std::thread::hardware_concurrency (), // clientThreadPoolSize
        65535, // maxRecvSize
        true, // sloppySocketRestart
        1024 // streamedResponseBufferSize
    );
    std::cout << "[TS] Starting up server..." << std::endl;
    {
        HTStack::Server server (serverConfiguration);
        server.start ();
        std::cout << "[TS] Server started up, waiting for 5 seconds..." << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds (5));
        std::cout << "[TS] Slept for 5 seconds, shutting down server..." << std::endl;
        server.shutdown ();
    } // Makes sure ~HTStack::Server () is called before shutdown completion message is shown
    std::cout << "[TS] Server shut down." << std::endl;
};

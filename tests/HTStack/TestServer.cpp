#include "TestServer.hpp"
#include "../../src/ServerConfiguration/ServerConfiguration.hpp"
#include "../../src/Server/Server.hpp"
#include "../../src/SSL/SSLSetupVars.hpp"
#include "../../src/CInteropUtils/CInteropUtils.hpp"
#include "../../src/AppConfigLoader/StaticAppConfig.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <openssl/ssl.h>
#include <optional>

int main () {
    HTStack::ServerConfiguration serverConfiguration (
        HTStack::ServerConfiguration::AppSetupType::INLINE, // appSetupType
        std::vector <HTStack::StaticAppConfig> ({
            HTStack::StaticAppConfig (
                "TestApp", // name
                "TestApp.so", // location
                std::map <std::string, std::string> (), // settings
                true // isLoaded
            )
        }), // appStaticConfigs
        std::nullopt, // appConfigPath
        true, // autoStart
        "0.0.0.0", // ip
        8080, // port
        1024, // backlog
        std::thread::hardware_concurrency (), // clientThreadPoolSize
        65535, // maxRecvSize
        true, // sloppySocketRestart
        1024, // streamedResponseBufferSize
        HTStack::SSLSetupVars ( // use empty constructor to disable
            "cert.pem", // certChainFilePath
            "key.pem", // keyFilePath
            SSL_FILETYPE_PEM // keyFileType
        ) // ssl
    );
    std::cout << "[TS] Starting up server..." << std::endl;
    {
        HTStack::Server server (serverConfiguration);
        server.start ();
        std::cout << "[TS] Server started up, waiting for a SIGINT..." << std::endl;
        /*
        std::cout << "[TS] Server started up, waiting for 5 seconds..." << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds (5));
        std::cout << "[TS] Slept for 5 seconds, shutting down server..." << std::endl;
        */
        HTStack::CInteropUtils::waitForSignal (SIGINT);
        std::cout << "[TS] Got a SIGINT, shutting down server..." << std::endl;
        server.shutdown ();
    } // Makes sure ~HTStack::Server () is called before shutdown completion message is shown
    std::cout << "[TS] Server shut down." << std::endl;
};

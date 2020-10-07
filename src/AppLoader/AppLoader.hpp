#pragma once

#include "../AppConfigLoader/AppConfigLoader.hpp"

#include <vector>
#include <string>
#include <stdexcept>
#include <optional>
#include <mutex>

namespace HTStack {
    class Server;
    class AppContainer;
    class Request;
    class AppLoader {
        Server & server;
        std::vector <AppContainer*> apps;
        AppConfigLoader* appConfigLoader;
        std::mutex appAccessLock;

        void _setupTypeCheck (std::string const & operation);
        AppContainer* _find (std::string const & appName);

        friend void AppConfigLoader::load ();
        friend void AppConfigLoader::save ();
    public:
        explicit AppLoader (Server & server_);
        void start ();

        void list ();
        void add (std::string const & appName, std::string const & appLocation);
        void load (std::string const & appName);
        void show (std::string const & appName);
        void configure (std::string const & appName, std::string const & key, std::string const & value);
        void unconfigure (std::string const & appName, std::string const & key);
        void unload (std::string const & appName);
        void reload (std::string const & appName);
        void remove (std::string const & appName);

        void handleRequest (Request & request);

        void shutdown ();
        ~AppLoader ();
    };
};

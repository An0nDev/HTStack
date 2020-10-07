#pragma once
#include <string>
#include <map>

namespace HTStack {
    class App;
    class Server;
    typedef void* SharedObjectHandle;
    typedef App* (*AppFactory) (Server const & server_);
    class AppContainer {
    private:
        Server & server;
        SharedObjectHandle handle = nullptr;
    public:
        std::string name;
        std::string location;
        std::map <std::string, std::string> settings;
        bool isLoaded = false;
        App* app = nullptr;
        AppContainer (
            Server & server_,
            std::string const & name_,
            std::string const & location_,
            std::map <std::string, std::string> const & settings_,
            bool const & isLoaded_
        );
        void load ();
        void unload ();
        ~AppContainer ();
    };
};

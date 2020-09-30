#pragma once
#include <string>
#include <map>

namespace HTStack {
    class App;
    class Server;
    typedef void* SharedObjectHandle;
    typedef App* (*AppFactory) ();
    class AppContainer {
    private:
        SharedObjectHandle handle = nullptr;
    public:
        std::string name;
        std::string location;
        std::map <std::string, std::string> settings;
        bool isLoaded = false;
        App* app = nullptr;
        explicit AppContainer (std::string const & location_);
        void load (Server & server);
        void unload ();
        ~AppContainer ();
    };
};

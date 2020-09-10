#pragma once
#include <string>

namespace HTStack {
    class App;
    class Server;
    typedef void* SharedObjectHandle;
    typedef App* (*AppFactory) ();
    class AppContainer {
    private:
        SharedObjectHandle handle = nullptr;
    public:
        std::string location;
        bool isLoaded = false;
        App* app = nullptr;
        explicit AppContainer (std::string const & location_);
        void load (Server & server);
        void unload ();
        ~AppContainer ();
    };
};

#pragma once
#include <string>
#include <stdexcept>

namespace HTStack {
    class App;
    class Server;
    typedef void* SharedObjectHandle;
    typedef App* (*AppFactory) ();
    class AppContainer {
    private:
        SharedObjectHandle handle;
    public:
        std::string location;
        bool isLoaded = false;
        App* app = nullptr;
        AppContainer (std::string const & location_);
        void load (Server & server);
        void unload ();
        ~AppContainer () throw (std::logic_error);
    };
};

#include "AppContainer.hpp"
#include "dlfcn.h"
#include "../App/App.hpp"

namespace HTStack {
    AppContainer::AppContainer (std::string const & location_)
    : location (location_) {};
    void AppContainer::load (Server & server) {
        if (isLoaded) {
            unload ();
        }
        handle = dlopen (location.data (), RTLD_NOW); // resolve all symbols immediately
        if (handle == nullptr) throw std::logic_error (location);
        void* factoryVoidPointer = dlsym (handle, "factory");
        if (factoryVoidPointer == nullptr) throw std::logic_error (location + " factory");
        AppFactory factory = reinterpret_cast <AppFactory> (factoryVoidPointer);
        app = factory ();
        app->server = &server;
        app->onLoad ();
        isLoaded = true;
    };
    void AppContainer::unload () {
        if (!isLoaded) {
            return;
        }
        isLoaded = false;
        app->onUnload ();
        delete app;
        dlclose (handle);
    };
    AppContainer::~AppContainer () noexcept (false) {
        if (isLoaded) {
            throw std::logic_error ("~AppContainer () called with isLoaded set; unload () should be unloaded before AppContainer destructs!");
        };
    };
};

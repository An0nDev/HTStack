#include "AppContainer.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "dlfcn.h"
#include "../App/App.hpp"

namespace HTStack {
    AppContainer::AppContainer (std::string const & name, std::string const & location_, std::map <std::string, std::string> const & settings, bool const & isLoaded_)
    : name (name_), location (location_), settings (settings_) {
        if (isLoaded_) load ();
    };
    void AppContainer::load (Server & server) {
        if (isLoaded) {
            unload ();
        }
        dlerror (); // Clear previous errors
        handle = dlopen (location.data (), RTLD_NOW); // resolve all symbols immediately
        if (handle == nullptr) {
            char* error = dlerror ();
            if (error == nullptr) throw std::runtime_error ("dlopen () failed, but dlerror () reports no errors thrown");
            throw std::logic_error ("Error when opening dynamic library at " + location + ": " + *error);
        }
        void* factoryVoidPointer = dlsym (handle, "factory");
        if (factoryVoidPointer == nullptr) {
            char* error = dlerror ();
            if (error == nullptr) throw std::runtime_error ("dlsym () failed, but dlerror () reports no errors thrown");
            throw std::logic_error ("Error when finding factory function in dynamic library at " + location + ": " + *error);
        }
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
        dlerror (); // Clear the error first
        dlclose (handle);
        char* error = dlerror ();
        if (error != nullptr) throw std::runtime_error ("Error when closing dynamic library at " + location + ": " + error);
    };
    AppContainer::~AppContainer () {
        if (isLoaded) {
            unload ();
        };
    };
};

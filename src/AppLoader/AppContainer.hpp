#pragma once
#include <string>
#include <map>
#include <optional>
#include <filesystem>

namespace HTStack {
    class App;
    class Server;
    typedef void* SharedObjectHandle;
    typedef App* (*AppFactory) (Server & server_, std::map <std::string, std::string> & settings_);
    class AppContainer {
    private:
        Server & server;
        SharedObjectHandle handle = nullptr;
    public:
        std::string name;
        std::string location;
        std::optional <std::filesystem::path> locationDirectory;
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

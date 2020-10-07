#pragma once
#include "../Server/Server.hpp"

namespace HTStack {
    class Server;
    class Request;
    class App {
    protected:
        Server & server;
        std::map <std::string, std::string> & settings;
    public:
        App (Server & server_, std::map <std::string, std::string> & settings_);
        virtual void handleSettingsUpdate (std::string const & key);
        virtual void handleSettingsRemove (std::string const & key);
        virtual void handleRequest (Request & request);
        virtual ~App ();
    };
};

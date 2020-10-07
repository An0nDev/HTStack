#include "App.hpp"

namespace HTStack {
    App::App (Server & server_, std::map <std::string, std::string> & settings_) : server (server_), settings (settings_) {};
    void App::handleSettingsUpdate (std::string const & key) {};
    void App::handleSettingsRemove (std::string const & key) {};
    void App::handleRequest (Request & request) {};
    App::~App () {};
};

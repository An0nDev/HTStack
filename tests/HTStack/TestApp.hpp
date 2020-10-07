#include "../../src/App/App.hpp"
#include "../../src/Server/Server.hpp"
#include "../../src/Request/Request.hpp"
#include <map>
#include <string>

class TestApp : public HTStack::App {
public:
    TestApp (HTStack::Server & server, std::map <std::string, std::string> & settings);
    void handleRequest (HTStack::Request & request);
    void handleSettingsUpdate (std::string const & key);
    void handleSettingsRemove (std::string const & key);
    ~TestApp ();
};

extern "C" HTStack::App* factory (HTStack::Server & server, std::map <std::string, std::string> & settings);

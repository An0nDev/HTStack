#include "../../src/App/App.hpp"
#include "../../src/Server/Server.hpp"
#include "../../src/Request/Request.hpp"

class TestApp : public HTStack::App {
public:
    TestApp (HTStack::Server & server);
    void handleRequest (HTStack::Request & request);
    ~TestApp ();
};

extern "C" HTStack::App* factory (HTStack::Server & server);

#include "../../src/App/App.hpp"
#include "../../src/Server/Server.hpp"
#include "../../src/Request/Request.hpp"

class TestApp : public HTStack::App {
public:
    TestApp ();
    void onLoad ();
    void onRequest (HTStack::Request & request);
    void onUnload ();
    ~TestApp ();
};

extern "C" HTStack::App* factory ();

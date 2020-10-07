### WARNING: This documentation is outdated! Some changes have been made to the internals of how an architecture works, and code samples may be partially broken.

## Getting Started with HTStack
DISCLAIMER: This document is a work-in-progress. Open an issue if you think something is missing or needs clarification. Thanks!

### Fast-track for experienced developers
Check out `tests/HTStack` for example code for a fully-functional HTStack architecture. This code is used during development, so it uses the majority of HTStack's features.

### A tour of the architecture concept
#### Developer's rant
I've written a lot of HTTP code using a variety of languages, and one thing I've seen that's common is that the separation between development code and production code is almost always created by the developer directly. What I mean is that you write your code in development, and to deploy it, you're forced to add special code that handles credential management and find a way to host a development and production server separately. Not only that, but you also often lose key debugging features when you switch to "production mode", making it harder to track down user-found bugs, or, even worse, are forced to adhere to some strict set of rules for production code, which can bloat your codebase and confuse other team members. I personally hate seeing the text "Development server: do not use in production!" because I know it means that after I've finished the heavy lifting of writing the application, then I have to spend time writing *more* code just to run the same application in production mode, which usually has security or efficiency benefits. HTStack solves this issue by allowing several applications to run contiguously using the same API. That is, you simply write your application once, and deploy two separate instances of it, one for development and one for production. HTStack supports real-time reloading of applications, so as development progresses, if you run into issues, simply reload the development instance, leaving the production instance untouched. When you're satisfied with your changes and are ready to push your code, all you do is copy the development binary onto the production binary, and reload the production instance. These are stored as shared objects so it's just one file that you're deploying, not your entire codebase.

#### How the architecture works
An architecture in terms of HTStack consists of one server and one or more apps.

A server is an instance of an `HTStack::Server` object in C++ that handles the low-level socket communication and HTTP protocol interpretation. The constructor for the object takes a list of applications that the server should manage. This list, as well as other configuration variables, are provided as an `HTStack::ServerConfiguration` instance. (A method of configuring these variables in a file is a planned feature.)

Each application consists primarily of an object that inherits from an `HTStack::App` object. This object is defined by the developer to have implementations of virtual functions that are called when various events happen, namely:
- `MyApp::MyApp (HTStack::Server &)`: called when the application's shared object is loaded into memory
- `void MyApp::handleRequest (HTStack::Request &)`: called when the server receives a valid HTTP request for the app to handle
- `MyApp::~MyApp ()`: called when the application's shared object is unloaded from memory

given the name of the object inheriting from `HTStack::App` is named `MyApp`.

A configuration file determines whether or not the application is loaded by default when the server starts up. When the server is running, a terminal interface provides a mechanism of changing this behavior. See "How to dynamically configure your app" for more info.

#### How to create an app
Apps are relatively simple to compile and run. The code for your app should contain a definition for a class that inherits from `HTStack::App` and a single function `extern "C" HTStack::App* factory (HTStack::Server & server)` that returns a pointer to a heap-allocated instance of your class. This function is called to construct your app when it's designated to be loaded, and the virtual destructor is called with `delete` when your app is designated to be unloaded.

Here's a minimal example, adapted from the one in `tests/HTStack`:

```cpp
// These assume your example is placed next to an HTStack folder which is a clone of the repository
#include "HTStack/src/App/App.hpp"
#include "HTStack/src/Request/Request.hpp"
#include "HTStack/src/Response/Response.hpp"

#include <iostream>

class TestApp : public HTStack::App {
public:
    TestApp (HTStack::Server & server) : App (server) {
        // construction goes here
    };
    void handleRequest (HTStack::Request & request) {
        // The request should be responded to using HTStack::Response::respondTo;
        // otherwise the connection is closed without any data being sent
    };
    ~TestApp () {
        // Deconstruction goes here
    };
};

extern "C" HTStack::App* factory (HTStack::Server & server) {
    return new TestApp (server);
};
```

Note that in a real implementation, the declaration and definition of your class and the `factory` function should be placed in .hpp and .cpp files; they're combined here for brevity.

When compiling your code, make sure to use the `-fPIC` and `-shared` flags to make sure you get a shared object file that can then be loaded by `HTStack::Server`, which calls `dlopen` and `dlsym` under the hood. Of course, you'll also need to compile `App/App.cpp`, `Request/Request.cpp`, `Response/Response.cpp`, and any other code from HTStack that you use in your code. HTStack also uses C++20 Features, so specify `-std=c++2a` when compiling your app if that's not your compiler's default.

Here's a minimal example of a simple app compilation script, copied from the one in `tests/HTStack`:
```bash
clang++-11 \
    -std=c++2a \
    -fPIC -shared \
    $SRC_DIR/App/App.cpp \
    $SRC_DIR/Request/Request.cpp \
    $SRC_DIR/Response/Response.cpp \
    $SRC_DIR/HTTPUtils/MIMEType.cpp \
    $SRC_DIR/CInteropUtils/CInteropUtils.cpp \
    TestApp.cpp -o TestApp.so
```

### API documentation
API docs are not yet available, but you can look at the header files in `src/*`, especially `Request/Request.hpp` and `Response/Response.hpp`, to get a feel for the usage of each `HTStack` object.

### What to do if your functionality is missing
Read over README.md.

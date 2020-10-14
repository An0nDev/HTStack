### WARNING: This documentation is outdated! Some changes have been made to the internals of how an architecture works, and code samples may be partially broken.

## Getting Started with HTStack
DISCLAIMER: This document is a work-in-progress. Open an issue if you think something is missing or needs clarification. Thanks!

### Fast-track for experienced developers
Check out `tests/HTStack` for example code for a fully-functional HTStack architecture. This code is used during development, so it uses the majority of HTStack's features.

### A tour of the architecture concept
#### Developer's rant
I've written a lot of HTTP code using a variety of languages, and one thing I've seen that's common is that the separation between development code and production code is almost always created by the developer directly. What I mean is that you write your code in development, and to deploy it, you're forced to add special code that handles credential management and find a way to host a development and production server separately. Not only that, but you also often lose key debugging features when you switch to "production mode", making it harder to track down user-found bugs, or, even worse, are forced to adhere to some strict set of rules for production code, which can bloat your codebase and confuse other team members. I personally hate seeing the text "Development server: do not use in production!" because I know it means that after I've finished the heavy lifting of writing the application, then I have to spend time writing *more* code just to run the same application in production mode, which usually has security or efficiency benefits. HTStack solves this issue by allowing several applications to run contiguously using the same API. That is, you simply write your application once, and deploy two separate instances of it, one for development and one for production. HTStack supports real-time reloading of applications, so as development progresses, if you run into issues, simply reload the development instance, leaving the production instance untouched. When you're satisfied with your changes and are ready to push your code, all you do is copy the development binary onto the production binary, and reload the production instance. These are stored as shared objects so it's just one file that you're deploying, not your entire codebase.

#### How the architecture works
An architecture in terms of HTStack consists of one server and one or more apps. The server and include/library files needed for app development are installed when you run `build/Build.sh` in this repository.

The server uses several classes provided by HTStack and is installed into /usr/local/bin/ as `HTStackServer`. It must be run with a singular argument: the path to the server config file.

Include files are installed to `/usr/local/include` as a folder called HTStack that contains the source tree, and a library file is installed to `/usr/local/lib` as a shared object called `libHTStack.so`. The include directory should be picked up automatically by clang, but to tell clang to link your app against the library, you will need to add `-lHTStack` to the argument list.

#### Setting up a server
##### The server config file
As mentioned before, when starting a server, the only command-line argument needs to be the path to a server config file. This file should have one configuration key and value per line, separated with a colon and a space. Here's an example server config file:
```
appConfigPath: TestAppConfig.cfg
autoStart: true
ip: 0.0.0.0
port: 8080
backlog: 1024
clientThreadPoolSize: auto
maxRecvSize: 65535
sloppySocketRestart: true
streamedResponseBufferSize: 1024
ssl: enable, cert.pem, key.pem, PEM
```
- `appConfigPath` is the path to the configuration file for any applications loaded. This is explained in-depth separately.
- `autoStart` is currently unused.
- `ip` is the IP the server will bind to. `0.0.0.0` specifies all interfaces.
- `port` is the port the server will bind to. Note that ports 0-1024 are reserved to the root user, so you will need to run `HTStackServer` with `sudo`.
- `backlog` is the number of connections the kernel will queue.
- `clientThreadPoolSize` is the number of threads used to handle connections from clients. `auto` calls `std::thread::hardware_concurrency ()`; integer values will be used as is.
- `maxRecvSize` is the maximum number of bytes received in one call to `recv`. `65535` is a good default since it's enough to encapsulate the data from a singular TCP packet.
- `sloppySocketRestart` toggles `SO_REUSEADDR`, which allows the server to restart more quickly, but may cause data loss if clients were still sending data when the server was closed. See https://stackoverflow.com/a/15199016/5037905 for more info.
- `streamedResponseBufferSize` defines the maximum amount of data that's read from the `istream` when a streamed response is used.
- `ssl` toggles and sets up SSL for the server. `disable` disables SSL; otherwise, exactly four comma-separated arguments should be specified: `enable`, the path to the certificate file, the path to the private key file, and either `PEM` or `ASN1` to specify the type of the private key file.

The recommended extension for the server config file is `.cfg`, but this is not enforced. **The server neither edits the file nor monitors the file for edits.** If you want to apply a change, you need to reload the server by typing `exit` at the HTStack prompt and then running it again.

##### The app config file
The server configuration key `appConfigPath` specifies the path to a separate application configuration file. This file provides a list of applications with enough information for the server to load and configure them. Here's an example app config file:
```
App1:
    _location: ../apps/App1.so
    _loaded: true
    configKey: configValue

App2:
    _location: ../apps/App2.so
    _loaded: false
    configKey2: configValue2

```

Each entry for an individual app starts with a name for that app, then several configuration lines with exactly four spaces in front, followed by a newline.

Only two configuration lines are required, `_location` and `_loaded`:
- `_location` provides a path to the shared object for the app to be loaded; this shared object should be placed there as part of the build process for the app. `_location` can also be a name of a shared object in a directory included in `LD_LIBRARY_PATH`, but it's recommended to state the path explicitly.
- `_loaded` simply denotes whether or not the app is loaded into memory using a boolean. Apps can exist in the configuration without being loaded for convenience, and an app's load state can be toggled while the server is running, allowing for runtime reloading of apps, intended for use in development environments. Note that the load state of an app will persist when the server restarts, and if an app fails to load, its load state is set to `false`.

**The server will not monitor the file for edits, but will edit the file when a change is made through the console.** Only edit the file manually when the server is closed to avoid undefined behavior.

Each application consists primarily of an object that inherits from an `HTStack::App` object. This object is defined by the developer to have implementations of virtual functions that are called when various events happen, namely:
- `MyApp::MyApp (HTStack::Server &, std::map <std::string, std::string> & settings)`: called when the application's shared object is loaded into memory; should call HTStack::App's constructor with the Server and std::map instances
- `void MyApp::handleRequest (HTStack::Request &)`: called when the server receives a valid HTTP request for the app to handle
- `void MyApp::handleSettingsUpdate (std::string const & key)`: called when a setting for this application has been updated
- `void MyApp::handleSettingsRemove (std::string const & key)`: called when a setting for this application has been removed
- `MyApp::~MyApp ()`: called when the application's shared object is unloaded from memory

given the name of the object inheriting from `HTStack::App` is named `MyApp`.

An application configuration file, the path to which should be placed in the server configuration file, determines whether or not the application is loaded by default when the server starts up. When the server is running, a terminal interface provides a mechanism of changing this behavior. See "How to dynamically configure your app" for more info.

#### How to create an app
Apps are relatively simple to compile and run. The code for your app should contain a definition for a class that inherits from `HTStack::App` and a single function `extern "C" HTStack::App* factory (HTStack::Server & server, std::map <std::string, std::string> & settings)` that returns a pointer to a heap-allocated instance of your class. The two arguments passed to `factory` should be passed directly to the constructor of your class. This function is called to construct your app when it's designated to be loaded, and the virtual destructor is called with `delete` when your app is designated to be unloaded.

Here's a minimal example, adapted from the one in `tests/HTStack`:

```cpp
#include <HTStack/App/App.hpp>
#include <HTStack/Request/Request.hpp>
#include <HTStack/Response/Response.hpp>

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

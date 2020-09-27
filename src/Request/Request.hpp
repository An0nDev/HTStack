#pragma once

#include "../ClientSocket/ClientSocket.hpp"

#include <map>
#include <optional>

namespace HTStack {
    class Server;
    class Request {
    public:
        Server const & server;
        ClientSocket* clientSocket;
        enum Method {
            NONE    = -1, // do not use! may break things
            GET     = 0, HEAD   = 1, POST = 2,
            PUT     = 3, DELETE = 4, CONNECT = 5,
            OPTIONS = 6, TRACE  = 7, PATCH = 8
        };
        Method method;
        static std::optional <Method> methodStringToEnum (std::string const & methodString);
        std::string path;
        std::map <std::string, std::string> headers;
        bool complete = false;
        Request (
            Server const & server_,
            ClientSocket* const & clientSocket_,
            Method const & method_,
            std::string const & path_,
            std::map <std::string, std::string> const & headers_
        );
    };
};

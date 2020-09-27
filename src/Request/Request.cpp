#include "Request.hpp"

namespace HTStack {
    std::optional <Request::Method> Request::methodStringToEnum (std::string const & methodString) {
        #define check(NAME) if (methodString == #NAME) { return std::optional <Request::Method> {Request::Method::NAME}; }

        check(GET)     else check(HEAD)   else check(POST) else
        check(PUT)     else check(DELETE) else check(CONNECT) else
        check(OPTIONS) else check(TRACE)  else check(PATCH)
        else {return std::nullopt;}
    };
    Request::Request (
        Server const & server_,
        ClientSocket* const & clientSocket_,
        Method const & method_,
        std::string const & path_,
        std::map <std::string, std::string> const & headers_
    ) : server (server_), clientSocket (clientSocket_), method (method_), path (path_), headers (headers_) {};
};

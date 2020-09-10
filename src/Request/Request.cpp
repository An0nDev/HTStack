#include "Request.hpp"

namespace HTStack {
    std::optional <Request::Method> Request::methodStringToEnum (std::string const & methodString) {
        std::string const & ms = methodString;
        Request::Method method = Request::Method::NONE;
        Request::Method & m = method;

        #define check(NAME) if (ms == #NAME) { method = Request::Method::NAME; }

        check(GET)     else check(HEAD)   else check(POST) else
        check(PUT)     else check(DELETE) else check(CONNECT) else
        check(OPTIONS) else check(TRACE)  else check(PATCH)
        else {return std::nullopt;}
        return std::optional <Request::Method> {method};
    };
    Request::Request (
        int const & clientSocket_,
        Method const & method_,
        std::string const & path_,
        std::map <std::string, std::string> const & headers_
    ) : clientSocket (clientSocket_), method (method_), path (path_), headers (headers_) {};
};

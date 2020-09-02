#include "Request.hpp"

namespace HTStack {
    std::optional <Request::Method> Request::methodStringToEnum (std::string const & methodString) {
        std::string const & ms = methodString;
        Request::Method method = Request::Method::NONE;
        Request::Method & m = method;

        if (ms == "GET")     m = Request::Method::GET;
        if (ms == "HEAD")    m = Request::Method::HEAD;
        if (ms == "POST")    m = Request::Method::POST;
        if (ms == "PUT")     m = Request::Method::PUT;
        if (ms == "DELETE")  m = Request::Method::DELETE;
        if (ms == "CONNECT") m = Request::Method::CONNECT;
        if (ms == "OPTIONS") m = Request::Method::OPTIONS;
        if (ms == "TRACE")   m = Request::Method::TRACE;
        if (ms == "PATCH")   m = Request::Method::PATCH;

        if (method == Request::Method::NONE) {
            return std::nullopt;
        } else {
            return std::optional <Request::Method> {method};
        }
    };
    Request::Request (
        int const & clientSocket_,
        Method const & method_,
        std::string const & path_,
        std::map <std::string, std::string> const & headers_
    ) : clientSocket (clientSocket_), method (method_), path (path_), headers (headers_) {};
};

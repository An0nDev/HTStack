#include "Response.hpp"
#include <iostream>

namespace HTStack {
    void Response::sendTo (int const & clientSocket) {
        std::cout << "TODO: send response to " << clientSocket << std::endl;
    };
    Response::Response (int const & responseCode_)
    : responseCode (responseCode_) {};
    Response::Response (int const & responseCode_, std::map <std::string, std::string> const & headers_)
    : responseCode (responseCode_), headers (headers_) {};
    Response::Response (int const & responseCode_, std::vector <char> const & data_)
    : responseCode (responseCode_), data (data_) {};
    void Response::respondTo (Request const & request) {
        sendTo (request.clientSocket);
    };
};

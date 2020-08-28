#include "RequestReader.hpp"
#include <iostream>
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"

namespace HTStack {
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    Request RequestReader::readFrom (int const & clientSocket, sockaddr_in const & clientAddress) {
        std::cout << "Reading from " << clientSocket << std::endl;
        return Request ();
    };
};

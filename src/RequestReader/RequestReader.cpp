#include "RequestReader.hpp"
#include <iostream>
#include "../Server/Server.hpp"
#include "../Request/Request.hpp"

namespace HTStack {
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    Request RequestReader::readFrom (int const & sockfd) {
        std::cout << "Reading from " << sockfd << std::endl;
        return Request ();
    };
};

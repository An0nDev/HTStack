#pragma once
#include <netinet/ip.h>

namespace HTStack {
    class Server;
    class Request;
    class RequestReader {
    public:
        Server & server;
        RequestReader (Server & server_);
        Request readFrom (int const & clientSocket, sockaddr_in const & clientAddress);
    };
};

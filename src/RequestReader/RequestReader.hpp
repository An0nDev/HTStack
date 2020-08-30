#pragma once

#include <netinet/ip.h>
#include <vector>
#include <optional>
#include "../Request/Request.hpp"

namespace HTStack {
    class Server;
    class RequestReader {
    private:
        static const std::string CRLF;
        static const std::string headerNameAndValueSeparator;
    public:
        Server & server;
        RequestReader (Server & server_);
        std::optional <Request> readFrom (int const & clientSocket, sockaddr_in const & clientAddress);
    };
};

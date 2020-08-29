#pragma once

#include <netinet/ip.h>
#include <vector>
#include <optional>

namespace HTStack {
    class Server;
    class Request;
    class RequestReader {
    private:
        static const std::string CRLF;
        std::optional <std::vector <char>> recv_ (int const & clientSocket);
        std::string leftoverFromLastRecvUntil;
        std::optional <std::string> recvUntil_ (int const & clientSocket, std::string endPattern);
    public:
        Server & server;
        RequestReader (Server & server_);
        std::optional <Request> readFrom (int const & clientSocket, sockaddr_in const & clientAddress);
    };
};

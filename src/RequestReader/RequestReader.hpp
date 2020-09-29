#pragma once

#include "../Request/Request.hpp"
#include "../ClientSocket/ClientSocket.hpp"

#include <netinet/ip.h>
#include <vector>
#include <optional>

namespace HTStack {
    class Server;
    class RequestReader {
    private:
        static const std::string CRLF;
        static const std::string headerNameAndValueSeparator;
        static const std::string methodAndPathSeparator;
        static const std::string pathAndVersionSeparator;
    public:
        Server & server;
        explicit RequestReader (Server & server_);
        Request readFrom (ClientSocket* const & clientSocket);
    };
};

#pragma once

#include "../ClientSocket/ClientSocket.hpp"

#include <optional>
#include <vector>
#include <string>

namespace HTStack {
    class Server;
    class InternalReader {
    private:
        std::vector <char> leftoverFromLastCall;
        Server const & server;
        ClientSocket* const & clientSocket;
        std::vector <unsigned char> recv ();
    public:
        InternalReader (Server const & server_, ClientSocket* const & clientSocket_);
        std::optional <std::vector <unsigned char>> recvData (int const & length);
        std::string recvTextUntil (std::string const & endPattern);
        ~InternalReader ();
    };
};

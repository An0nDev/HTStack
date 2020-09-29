#pragma once

#include "../ClientSocket/ClientSocket.hpp"

#include <optional>
#include <vector>

namespace HTStack {
    class Server;
    class InternalReader {
    private:
        std::vector <char> leftoverFromLastCall;
        Server const & server;
        ClientSocket* const & clientSocket;
        std::vector <char> recv ();
    public:
        InternalReader (Server const & server_, ClientSocket* const & clientSocket_);
        std::optional <std::vector <char>> recvData (int const & length);
        std::string recvTextUntil (std::string const & endPattern);
        ~InternalReader ();
    };
};

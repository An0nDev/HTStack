#pragma once

#include "../ClientSocket/ClientSocket.hpp"

#include <netinet/ip.h>
#include <vector>

namespace HTStack {
    class ClientThreadTask {
    public:
        ClientSocket* clientSocket;
        ClientThreadTask (ClientSocket* const & clientSocket_);
    };
};

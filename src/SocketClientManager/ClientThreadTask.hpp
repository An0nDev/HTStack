#pragma once

#include <netinet/ip.h>
#include <vector>

namespace HTStack {
    class ClientThreadTask {
    public:
        int clientSocket;
        sockaddr_in clientAddress;
        ClientThreadTask (int const & clientSocket_, sockaddr_in const & clientAddress_);
    };
};

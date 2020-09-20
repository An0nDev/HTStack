#include <netinet/ip.h>

namespace HTStack {
    class ClientThreadTask {
        int clientSocket;
        sockaddr_in clientAddress;
        ClientThreadTask (int const & clientSocket_, sockaddr_in const & clientAddress_);
    };
};

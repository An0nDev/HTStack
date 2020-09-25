#include "SSLSocket.hpp"
#include <stdexcept>

namespace HTStack {
    SSLSocket::SSLSocket (int clientSocketFileDescriptor, SSL_CTX* sslServer)
    : ClientSocket (clientSocketFileDescriptor), sslClient (SSL_new (sslServer)) {
        if (sslClient == nullptr) {
            // Find an alternative to ERR_print_errors_fp that prints to a string,
            // and throw an exception associated with that.
            throw std::runtime_error ("Unable to create SSL client");
        }
        SSL_set_fd (sslClient, fd);
    }
    SSLSocket::read (int const & maxSize) {
        char buffer [maxSize];
        // Do shit here idk
        // sslClient->
    };
};

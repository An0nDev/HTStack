#include "SSLSocket.hpp"

#include "../ExceptionUtils/ExceptionUtils.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>

namespace HTStack {
    SSLSocket::SSLSocket (int const & clientSocketFileDescriptor, sockaddr_in const & clientSocketAddress, SSL_CTX* const & sslServer)
    : ClientSocket (clientSocketFileDescriptor, clientSocketAddress), sslClient (sslServer) {
        sslClient.set_fd (fd);
        sslClient.accept ();
    };
    std::vector <char> SSLSocket::read (int const & maxSize) {
        char buffer [maxSize];
        size_t readBytes;
        sslClient.read_ex (&buffer, maxSize, &readBytes);
        return std::vector <char> (buffer, buffer + readBytes);
    };
    void SSLSocket::write (std::vector <char> const & data) {
        sslClient.write ((void*) data.data (), data.size ());
    };
    SSLSocket::~SSLSocket () {
        try {
            sslClient.shutdown ();
        } catch (std::exception const & exception) {
            ExceptionUtils::safeThrow (exception);
        }
    };
};

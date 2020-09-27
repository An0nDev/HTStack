#include "SSLSocket.hpp"

#include "../ExceptionUtils/ExceptionUtils.hpp"

#include <stdexcept>
#include <exception>
#include <iostream>

namespace HTStack {
    SSLSocket::SSLSocket (int const & clientSocketFileDescriptor, sockaddr_in const & clientSocketAddress, SSL_CTX* const & sslServer)
    : ClientSocket (clientSocketFileDescriptor, clientSocketAddress) {
        sslClient = SSL_new (sslServer);
        if (sslClient == nullptr) {
            // Find an alternative to ERR_print_errors_fp that prints to a string,
            // and throw an exception associated with that.
            ERR_print_errors_fp (stderr); // Print errors to standard error
            throw std::runtime_error ("Unable to create SSL client");
        }
        if (SSL_set_fd (sslClient, fd) == 0) {
            ERR_print_errors_fp (stderr); // Print errors to standard error
            throw std::runtime_error ("Unable to set file descriptor of SSL client");
        }
        int acceptReturnValue (SSL_accept (sslClient));
        if (acceptReturnValue == -1 || acceptReturnValue == 0) {
            ERR_print_errors_fp (stderr); // Print errors to standard error
            throw std::runtime_error ("Unable to accept connection on SSL client");
        }
    }
    std::vector <char> SSLSocket::read (int const & maxSize) {
        char buffer [maxSize];
        size_t readBytes;
        std::cout << "sslClient is " << sslClient << std::endl;
        if (SSL_read_ex (sslClient, &buffer, maxSize, &readBytes) == 0) {
            ERR_print_errors_fp (stderr); // Print errors to standard error
            throw std::runtime_error ("Unable to read from SSL client");
        }
        return std::vector <char> (buffer, buffer + readBytes);
    };
    void SSLSocket::write (std::vector <char> const & data) {
        const char* dataPointer (reinterpret_cast <const char*> (data.data ()));
        if (SSL_write (sslClient, data.data (), data.size ()) == 0) {
            ERR_print_errors_fp (stderr); // Print errors to standard error
            throw std::runtime_error ("Unable to write to SSL client");
        }
    }
    SSLSocket::~SSLSocket () {
        int shutdownReturnValue (SSL_shutdown (sslClient));
        if (shutdownReturnValue != 0 && shutdownReturnValue != 1) {
            ERR_print_errors_fp (stderr); // Print errors to standard error
            // Since this is a destructor, check whether or not we already have an exception
            ExceptionUtils::safeThrow (std::runtime_error ("Unable to shut down SSL client"));
        }
    }
};

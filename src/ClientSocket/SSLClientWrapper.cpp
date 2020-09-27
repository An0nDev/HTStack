#include "SSLClientWrapper.hpp"

#include <openssl/err.h>

#include <stdexcept>
#include <iostream>

namespace HTStack {
    SSLClientWrapper::SSLClientWrapper (SSL_CTX* const & sslServer) {
        client = SSL_new (sslServer);
        if (client == nullptr) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to initialize SSL client");
        }
    };
    void SSLClientWrapper::set_fd (int const & fd) {
        if (SSL_set_fd (client, fd) == 0) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to set file descriptor of SSL client");
        }
    };
    void SSLClientWrapper::accept () {
        int acceptReturnValue (SSL_accept (client));
        if (acceptReturnValue < 0) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to accept a connection on an SSL client (fatal error)");
        } else if (acceptReturnValue == 0) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to accept a connection on an SSL client (controlled shutdown)");
        }
    };
    void SSLClientWrapper::read_ex (void* const & buffer, size_t const & maxSize, size_t* const & readBytes) {
        if (SSL_read_ex (client, buffer, maxSize, readBytes) == 0) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to read from SSL client");
        }
    };
    void SSLClientWrapper::write (void* const & buffer, size_t const & size) {
        if (SSL_write (client, buffer, size) <= 0) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to write to SSL client");
        }
    };
    void SSLClientWrapper::shutdown () {
        int shutdownReturnValue (SSL_shutdown (client));
        if (shutdownReturnValue != 0 && shutdownReturnValue != 1) {
            ERR_print_errors_fp (stderr);
            throw std::runtime_error ("Unable to shut down SSL client");
        }
    };
    SSLClientWrapper::~SSLClientWrapper () {
        SSL_free (client);
    };
};

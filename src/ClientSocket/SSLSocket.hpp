#include "ClientSocket.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace HTStack {
    class SSLSocket : ClientSocket {
    private:
        SSL* sslClient;
        SSLSocket (int clientSocketFileDescriptor, SSL_CTX* sslServer);
        read (int const & maxSize);
        write (std::vector <char> const & data);
        ~SSLSocket ();
    };
};

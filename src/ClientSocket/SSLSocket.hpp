#include "ClientSocket.hpp"

#include "SSLClientWrapper.hpp"

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace HTStack {
    class SSLSocket : public ClientSocket {
    private:
        SSLClientWrapper sslClient;
    public:
        SSLSocket (int const & clientSocketFileDescriptor, sockaddr_in const & clientSocketAddress, SSL_CTX* const & sslServer);
        std::vector <unsigned char> read (int const & maxSize);
        void write (std::vector <unsigned char> const & data);
        ~SSLSocket ();
    };
};

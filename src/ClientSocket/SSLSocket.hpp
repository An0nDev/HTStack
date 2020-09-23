#include "ClientSocket.hpp"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace HTStack {
    class SSLSocket : ClientSocket {
        SSLSocket (int clientSocketFileDescriptor);
    };
};

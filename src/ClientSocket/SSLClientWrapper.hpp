#include <openssl/ssl.h>

namespace HTStack {
    class SSLClientWrapper {
    private:
        SSL* client = nullptr;
    public:
        SSLClientWrapper (SSL_CTX* const & sslServer);
        void set_fd (int const & fd);
        void accept ();
        void read_ex (void* const & buffer, size_t const & maxSize, size_t* const & readBytes);
        void write (void* const & buffer, size_t const & size);
        void shutdown ();
        ~SSLClientWrapper ();
    };
};

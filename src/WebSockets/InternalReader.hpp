#include "../ClientSocket/ClientSocket.hpp"
#include <cstdint>

namespace HTStack::WebSockets {
    class InternalReader {
        ClientSocket* clientSocket;
        std::vector <unsigned char> buffer;
    public:
        InternalReader (ClientSocket* const & clientSocket_);
        std::vector <unsigned char> read (uint_fast64_t const & length);
        ~InternalReader ();
    };
};

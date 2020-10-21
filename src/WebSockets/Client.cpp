#include "Client.hpp"

namespace HTStack::WebSockets {
    Client::Client (ClientSocket* const & underlyingClientSocket_)
    : underlyingClientSocket (underlyingClientSocket_) {};
};

#pragma once

#include "../ClientSocket/ClientSocket.hpp"
#include "DataFrame.hpp"

namespace HTStack::WebSockets {
    class ClientThread;
    class WebSocket {
        ClientSocket* clientSocket;
        bool closed;

        friend ClientThread;
        DataFrame recv_ ();
    public:
        WebSocket (ClientSocket* const & clientSocket_);
        /* void send (DataFrame const & dataFrame); */
        void close ();
        ~WebSocket ();
    };
};

#include "WebSocket.hpp"
#include "DataFrame.hpp"
#include "../ClientSocket/ClientSocket.hpp"

namespace HTStack::WebSockets {
    class DataFrameReader {
        static DataFrame readFrom (ClientSocket* const & clientSocket);
    };
};

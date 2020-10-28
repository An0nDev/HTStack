#include "WebSocket.hpp"
#include "DataFrameReader.hpp"
#include "DataFrameWriter.hpp"

namespace HTStack::WebSockets {
    WebSocket::WebSocket (ClientSocket* const & clientSocket_)
    : clientSocket (clientSocket_) {};
    DataFrame WebSocket::recv_ () {
        return DataFrameReader::readFrom (clientSocket);
    };
    void WebSocket::send (DataFrame const & dataFrame) {
        DataFrameWriter::writeTo (dataFrame, clientSocket);
    };
    void WebSocket::close () {
        if (!closed) {
            closed = true;
            delete clientSocket;
        }
    };
    WebSocket::~WebSocket () {
        close ();
    };
};

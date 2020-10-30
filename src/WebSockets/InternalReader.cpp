#include "InternalReader.hpp"

#include <iostream>

namespace HTStack::WebSockets {
    InternalReader::InternalReader (ClientSocket* const & clientSocket_)
    : clientSocket (clientSocket_) {};
    std::vector <unsigned char> InternalReader::read (uint_fast64_t const & length) {
        while (buffer.size () < length) {
            std::vector <unsigned char> newBuffer = clientSocket->read (length - buffer.size ());
            buffer.insert (buffer.end (), newBuffer.begin (), newBuffer.end ());
        };
        std::vector <unsigned char> out;
        out.insert (out.end (), buffer.begin (), buffer.begin () + length);
        buffer.erase (buffer.begin (), buffer.begin () + length);
        return out;
    };
    InternalReader::~InternalReader () {
        if (buffer.size () > 0) {
            std::cerr << "Warning from ~HTStack::WebSockets::InternalReader: leftover data in buffer!" << std::endl;
        }
    };
};

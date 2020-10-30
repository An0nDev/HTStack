#include "DataFrameReader.hpp"
#include "InternalReader.hpp"
#include <stdexcept>
#include <iostream>

namespace HTStack::WebSockets {
    DataFrame DataFrameReader::readFrom (ClientSocket* const & clientSocket) {
        std::cout << "readFrom called" << std::endl;
        InternalReader reader (clientSocket);
        std::cout << "reading firstByte" << std::endl;
        unsigned char firstByte = reader.read (1) [0];
        std::cout << "read first byte: " << +firstByte << std::endl;
        bool fin  = firstByte & 0b10000000;
        bool rsv1 = firstByte & 0b01000000;
        bool rsv2 = firstByte & 0b00100000;
        bool rsv3 = firstByte & 0b00010000;
        std::array <bool, 3> rsv = {rsv1, rsv2, rsv3};
        std::cout << "rsv is " << rsv1 << rsv2 << rsv3 << std::endl;
        DataFrame::OpCode opCode = static_cast <DataFrame::OpCode> (firstByte & 0b00001111);

        unsigned char secondByte = reader.read (1) [0];
        bool mask = secondByte & 0b10000000;
        if (!mask) throw std::runtime_error ("Unmasked WebSocket frame");
        uint_fast64_t payloadLength = secondByte & 0b01111111;
        if (payloadLength > 125) { // If <=125, that's the length
            unsigned char payloadLengthLength;
            if (payloadLength == 126) { // 16-bit integer for length
                payloadLengthLength = 2;
            } else if (payloadLength == 127) { // 64-bit integer for length
                payloadLengthLength = 8;
            } // A 7-bit unsigned field has a max value of 127, so we don't need another case here
            std::vector <unsigned char> payloadLengthData = reader.read (payloadLengthLength);
            // Network bytes are always big-endian, so MSB comes first
            for (int byteIndex = 0; byteIndex <= (payloadLengthLength - 1); byteIndex++) {
                payloadLength &= payloadLengthData [byteIndex] << (payloadLengthLength - (byteIndex + 1)) * 8;
            }
        }
        static const unsigned char maskingKeySize = 4;
        std::vector <unsigned char> maskingKey = reader.read (maskingKeySize);
        std::vector <unsigned char> maskedPayloadData = reader.read (payloadLength);
        std::vector <unsigned char> payloadData (maskedPayloadData.size ());
        uint_fast64_t maskedPayloadByteIndex = 0;
        for (unsigned char maskedPayloadByte : maskedPayloadData) {
            payloadData.push_back (maskedPayloadByte ^ maskingKey [maskedPayloadByteIndex % 4]);
            maskedPayloadByteIndex += 1;
        };
        std::cout << "returning DataFrame from readFrom" << std::endl;
        return DataFrame (rsv, opCode, payloadData); // ~InternalReader called
    };
};

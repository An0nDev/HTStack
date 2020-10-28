#include "DataFrameReader.hpp"
#include "InternalReader.hpp"

namespace HTStack::WebSockets {
    DataFrame DataFrameReader::readFrom (ClientSocket* const & clientSocket) {
        InternalReader reader (clientSocket);
        unsigned char firstByte = reader.read (1) [0];
        bool fin  = firstByte & 0b10000000;
        bool rsv1 = firstByte & 0b01000000;
        bool rsv2 = firstByte & 0b00100000;
        bool rsv3 = firstByte & 0b00010000;
        DataFrame::OpCode opCode = firstByte & 0b00001111;

        unsigned char secondByte = reader.read (1) [0];
        bool mask = secondByte & 0b10000000;
        if (!mask) throw std::runtime_error ("Unmasked WebSocket frame");
        uint_fast64_t payloadLength = secondByte & 0b01111111;
        if (payloadLength > 125) { // If <=125, that's the length
        const unsigned char payloadLengthLength;
            if (payloadLength == 126) { // 16-bit integer for length
                payloadLengthLength = 2;
            } else if (payloadLength == 127) { // 64-bit integer for length
                payloadLengthLength = 8;
            }
            std::vector <unsigned char> payloadLengthData = clientSocket->read (payloadLengthLength);
            // Network bytes are always big-endian, so MSB comes first
            for (int byteIndex = 0; byteIndex <= (payloadLengthLength - 1); byteIndex++) {
                payloadLength &= payloadLengthData [byteIndex] << (payloadLengthLength - (byteIndex + 1)) * 8;
            }
        }
        // Same as before but with a 4-byte integer
        static const unsigned char fourByteIntegerSize = 4;
        std::vector <unsigned char> maskingKeyData = reader.read (fourByteIntegerSize);
        uint_fast32_t maskingKey;
        for (int byteIndex = 0; byteIndex <= (fourByteIntegerSize - 1); byteIndex++) {
            maskingKey &= maskingKeyData [byteIndex] << (fourByteIntegerSize - (byteIndex + 1)) * 8;
        }
        std::vector <unsigned char> maskedPayloadData = reader.read (payloadLength);
        std::vector <unsigned char> payloadData (maskedPayloadData.size ());
        uint_fast64_t maskedPayloadByteIndex = 0;
        for (unsigned char maskedPayloadByte : maskedPayloadData) {
            payloadData.push_back (maskedPayloadByte ^ maskingKey [maskedPayloadByteIndex % 4]);
            maskedPayloadByteIndex += 1;
        };
        
    };
};

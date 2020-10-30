#include "Checker.hpp"
#include "../Response/Response.hpp"

#include <openssl/evp.h>
#include <openssl/sha.h>

#include <iostream>

namespace HTStack::WebSockets {
    const std::string Checker::supportedVersion = "13";
    const std::string Checker::keyDerivingMagicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    const unsigned int Checker::sha1HashBase64CharacterArrayLength = (
        ((SHA_DIGEST_LENGTH - (SHA_DIGEST_LENGTH % 3)) / 3) // length divided by 3 rounded down
         + ((SHA_DIGEST_LENGTH % 3 == 0) ? 0 : 4) // add extra four bytes if not divisible by 3
        + 1 // null terminator
    );
    std::string Checker::deriveKey (std::string const & clientKey) {
        // 1. Concatenate the client's Sec-WebSocket-Key and the magic string together.
        std::string concatenatedString = clientKey + keyDerivingMagicString;

        // 2. Take the SHA-1 hash of the result.
        // OpenSSL says: Applications should use the higher level functions
        // EVP_DigestInit(3) etc. instead of calling the hash functions directly.
        // Not sure why this is, but messing with OpenSSL contexts is a pain in the ass,
        // especially in any sort of RAII-desiring environment,
        // so I'm just calling the hash function directly... sorry, OpenSSL developers.
        unsigned char derivedHashRawBytes [SHA_DIGEST_LENGTH];
        SHA1 ((const unsigned char*) concatenatedString.c_str (), concatenatedString.size (), derivedHashRawBytes);

        // 3. Return the base64 encoding of that hash.
        unsigned char derivedHashBase64Bytes [sha1HashBase64CharacterArrayLength];
        EVP_EncodeBlock (derivedHashBase64Bytes, derivedHashRawBytes, SHA_DIGEST_LENGTH);
        return std::string ((char*) derivedHashBase64Bytes);
    };
    std::optional <WebSocket*> Checker::check (Request & request) {
        if (! (
            request.method == Request::Method::GET &&
            request.headers.contains ("Upgrade") &&
            request.headers ["Upgrade"] == "websocket" &&
            request.headers.contains ("Connection") &&
            request.headers ["Connection"] == "Upgrade"
        )) {
            return std::nullopt;
        }

        #define BAD() \
            Response response (400); \
            response.respondTo (request); \
            return std::nullopt

        if (!request.headers.contains ("Sec-WebSocket-Version")) { BAD(); }
        if (request.headers ["Sec-WebSocket-Version"] != supportedVersion) {
            Response response (400);
            response.headers ["Sec-WebSocket-Version"] = supportedVersion;
            response.respondTo (request);
        }
        if (!request.headers.contains ("Sec-WebSocket-Key")) { BAD(); }
        std::string clientKey (request.headers ["Sec-WebSocket-Key"]);
        std::string derivedKey (deriveKey (clientKey));
        Response response (101);
        response.headers ["Upgrade"] = "websocket";
        response.headers ["Connection"] = "Upgrade";
        response.headers ["Sec-WebSocket-Accept"] = derivedKey;
        response.respondTo (request);
        std::cout << "sent successful websocket response!" << std::endl;
        return new WebSocket (request.clientSocket);
    };
};

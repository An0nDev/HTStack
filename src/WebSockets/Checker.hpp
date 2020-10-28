#pragma once

#include "WebSocket.hpp"
#include "../Request/Request.hpp"
#include <optional>

namespace HTStack::WebSockets {
    class Checker {
        static const std::string supportedVersion;
        static const std::string keyDerivingMagicString;
        static const unsigned int sha1HashBase64CharacterArrayLength;
        std::string deriveKey (std::string const & clientKey);
    public:
        std::optional <WebSocket*> check (Request & request);
    };
};

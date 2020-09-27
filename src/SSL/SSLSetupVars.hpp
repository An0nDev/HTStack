#pragma once

#include <string>
#include <optional>
#include <openssl/ssl.h>

namespace HTStack {
    class SSLSetupVars {
    public:
        const bool enabled;
        // Only present if enabled is set
        const std::optional <std::string> certChainFilePath;
        const std::optional <std::string> keyFilePath;
        const std::optional <int> keyFileType;
        const std::optional <const SSL_METHOD*> method;
    public:
        SSLSetupVars ();
        SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_, int const & keyFileType_);
        SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_, int const & keyFileType_, const SSL_METHOD* method_);
    };
};

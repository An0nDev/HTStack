#include "SSLSetupVars.hpp"

namespace HTStack {
    SSLSetupVars::SSLSetupVars () :
    enabled (false), certChainFilePath (std::nullopt), keyFilePath (std::nullopt) {};
    SSLSetupVars::SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_, int const & keyFileType_) :
    enabled (true), certChainFilePath (certChainFilePath_), keyFilePath (keyFilePath_), keyFileType (keyFileType_), method (TLS_method ()) {};
    SSLSetupVars::SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_, int const & keyFileType_, const SSL_METHOD* method_) :
    enabled (true), certChainFilePath (certChainFilePath_), keyFilePath (keyFilePath_), keyFileType (keyFileType_), method (method_) {};
};

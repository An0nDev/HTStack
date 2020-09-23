#include "SSLSetupVars.hpp"
#include <string>
#include <optional>

namespace HTStack {
    SSLSetupVars::SSLSetupVars () :
    enabled (false), certChainFilePath (std::nullopt), keyFilePath (std::nullopt) {};
    SSLSetupVars::SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_) :
    enabled (true), certChainFilePath (certChainFilePath_), keyFilePath (keyFilePath_) {};
};

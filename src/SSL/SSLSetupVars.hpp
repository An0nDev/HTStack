#include <string>
#include <optional>

namespace HTStack {
    class SSLSetupVars {
    private:
        const bool enabled;
        // Only present if enabled is set
        const std::optional <std::string> certChainFilePath;
        const std::optional <std::string> keyFilePath;
    public:
        SSLSetupVars ();
        SSLSetupVars (std::string const & certChainFilePath_, std::string const & keyFilePath_);
    };
};

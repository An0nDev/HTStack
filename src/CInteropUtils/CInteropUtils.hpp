#pragma once

#include <system_error>

namespace HTStack {
    void __signalHandler (int signal);
    class CInteropUtils {
    public:
        static void systemErrorCheck (
            std::string const & systemFunctionName,
            int const & systemFunctionReturnValue
        );
        static void _handleSystemError (std::string const & systemFunctionName);
        static void waitForSignal (int const & signal);
    };
};

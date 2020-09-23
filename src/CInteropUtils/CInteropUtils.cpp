#include "CInteropUtils.hpp"

#include <errno.h>
#include <exception>
#include <iostream>
#include <unistd.h>
#include <signal.h>

namespace HTStack {
    void __signalHandler (int signal) {};
    void CInteropUtils::systemErrorCheck (
        std::string const & systemFunctionName,
        int const & systemFunctionReturnValue
    ) {
        if (systemFunctionReturnValue < 0) {
            _handleSystemError (systemFunctionName);
        }
    };
    void CInteropUtils::_handleSystemError (
        std::string const & systemFunctionName
    ) {
        std::system_error error (
            std::error_code (errno, std::system_category ()),
            systemFunctionName
        );
        if (std::uncaught_exceptions () > 0) {
            // An exception has already been thrown, so don't throw another one.
            // This case should only occur with destructors being called at the end of a scope.
            std::cerr << "System error with code " << error.code () << " (" << error.what () << ") was thrown when calling " << systemFunctionName << ".";
        } else {
            throw error;
        }
    }
    void CInteropUtils::waitForSignal (int const & signal_) {
        sighandler_t old = signal (signal_, (sighandler_t) __signalHandler); // Install dummy signal handler and save old one
        if (old == SIG_ERR) _handleSystemError ("signal ()");
        pause (); // Wait for the dummy signal handler to return
        // ^ No handling here because, according to the man page,
        // the function only returns "when a signal was caught
        // and the signal-catching function returned".
        signal (signal_, old); // Reinstall old signal handler
    };
}

#include "ExceptionUtils.hpp"

#include <iostream>

namespace HTStack {
    void ExceptionUtils::safeThrow (std::exception const & exception) {
        if (std::uncaught_exceptions () > 0) {
            std::cerr << "During destruction, another exception was thrown: " << exception.what () << std::endl;
        } else {
            throw std::runtime_error (exception.what ());
        }
    };
}

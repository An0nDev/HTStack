#include "CInteropUtils.hpp"

#include <errno.h>

namespace HTStack {
    void CInteropUtils::systemErrorCheck (
        std::string const & systemFunctionName,
        int const & systemFunctionReturnValue
    ) {
        if (systemFunctionReturnValue < 0) {
            throw std::system_error (
                std::error_code (errno, std::system_category ()),
                systemFunctionName
            );
        }
    };
}

#pragma once

#include <system_error>

namespace HTStack {
    class CInteropUtils {
    public:
        static void systemErrorCheck (
            std::string const & systemFunctionName,
            int const & systemFunctionReturnValue
        );
    };
};

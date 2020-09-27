#include <exception>

namespace HTStack {
    class ExceptionUtils {
    public:
        static void safeThrow (std::exception const & exception);
    };
};

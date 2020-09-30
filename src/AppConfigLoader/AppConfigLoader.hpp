#include <string>

namespace HTStack {
    class AppLoader;
    class AppConfigLoader {
    private:
        static const std::string keyValueSeparator (": ");
        static const std::string indent ("    ");
        AppLoader const & appLoader;
        const std::string filePath;
        AppConfigLoader (AppLoader const & appLoader_, std::string const & filePath_);
        void load ();
        // some sort of modify function goes here
        void save ();
        ~AppConfigLoader ();
    };
};

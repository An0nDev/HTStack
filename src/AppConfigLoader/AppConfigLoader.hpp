#pragma once

#include <string>

namespace HTStack {
    class AppLoader;
    class AppConfigLoader {
    private:
        static const std::string keyValueSeparator;
        static const std::string indent;
        AppLoader & appLoader;
        const std::string filePath;
    public:
        AppConfigLoader (AppLoader & appLoader_, std::string const & filePath_);
        void load ();
        void save ();
    };
};

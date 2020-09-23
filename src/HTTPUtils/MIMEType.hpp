#pragma once

#include <map>
#include <string>

namespace HTStack {
    class MIMEType {
    private:
        static const std::map <std::string, std::string> extensionsAndTypes;
        static const std::string defaultMIMEType;
        static const std::string fileNameDelimiter;
    public:
        MIMEType (std::string const & typeOrFileName_, bool const & guess_);
        std::string type;
        static std::string guess (std::string const & fileName);
    };
};

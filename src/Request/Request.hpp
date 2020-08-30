#pragma once

#include <map>

namespace HTStack {
    class Request {
    public:
        std::map <std::string, std::string> headers;
        bool complete = false;
    };
};

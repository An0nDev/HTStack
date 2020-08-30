#pragma once

#include <string>
#include <vector>

namespace HTStack {
    class Request {
    public:
        class Header {
        public:
            std::string name;
            std::string value;
            Header (std::string const & name_, std::string const & value_);
        };
        std::vector <Header> headers;
        bool complete = false;
    };
};

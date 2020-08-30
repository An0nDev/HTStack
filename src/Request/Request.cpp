#include "Request.hpp"

namespace HTStack {
    Request::Header::Header (std::string const & name_, std::string const & value_)
    : name (name_), value (value_) {};
};

#include "DataFrame.hpp"

namespace HTStack::WebSockets {
    DataFrame::DataFrame (
        std::array <bool, 3> const & rsv_,
        DataFrame::OpCode const & opCode_,
        DataFrame::Data const & data_
    ) : rsv (rsv_), opCode (opCode_), data (data_) {};
    DataFrame::Data::Data (std::string const & text_)
    : text (text_), type (Data::Type::TEXT) {};
    DataFrame::Data::Data (std::vector <unsigned char> const & binary_)
    : binary (binary_), type (Data::Type::BINARY) {};
    std::string DataFrame::Data::getText () {
        if (type !== Data::Type::TEXT) {
            throw std::logic_error ("Getting text from non-text frame");
        }
        return text.value ();
    };
    std::array <unsigned char> DataFrame::Data::getBinary () {
        if (type !== Data::Type::BINARY) {
            throw std::logic_error ("Getting binary from non-binary frame");
        }
        return binary.value ();
    };
};

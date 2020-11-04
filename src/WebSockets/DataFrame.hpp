#pragma once

#include <array>
#include <string>
#include <optional>
#include <vector>

namespace HTStack::WebSockets {
    class DataFrame {
    public:
        // We don't actually construct a frame if FIN !== 1,
        // so no need for a field here

        std::array <bool, 3> rsv; // Data for extensions

        enum OpCode {
            CONTINUATION = 0x0,
            TEXT = 0x1,
            BINARY = 0x2,
            CLOSE = 0x8,
            PING = 0x9,
            PONG = 0xA
        } opCode;
        bool isControl ();

        // Request will be ignored if not masked,
        // so no need for a field here

        class Data {
        private:
            std::optional <std::string> text;
            std::optional <std::vector <unsigned char>> binary;
            enum Type {
                TEXT,
                BINARY
            } type;
        public:
            Data (std::string const & text_);
            Data (std::vector <unsigned char> const & binary_);
            std::string getText ();
            std::vector <unsigned char> getBinary ();
        } data;

        DataFrame (
            std::array <bool, 3> const & rsv_,
            OpCode const & opCode_,
            Data const & data_
        );
    };
};

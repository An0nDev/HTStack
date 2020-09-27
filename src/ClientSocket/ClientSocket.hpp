#pragma once

#include <vector>
#include <optional>
#include <netinet/in.h>

namespace HTStack {
    class ClientSocket {
    protected:
        int fd;
        sockaddr_in address;
    public:
        bool keepOpen = false;
        ClientSocket (int const & fd_, sockaddr_in const & address_);
        virtual std::vector <char> read (int const & maxSize);
        virtual void write (std::vector <char> const & data);
        virtual ~ClientSocket ();
    };
};

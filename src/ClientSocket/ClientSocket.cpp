#include "ClientSocket.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"

#include <sys/socket.h>
#include <unistd.h>

namespace HTStack {
    ClientSocket::ClientSocket (int const & fd_, sockaddr_in const & address_) : fd (fd_), address (address_) {};
    std::vector <char> ClientSocket::read (int const & maxSize) {
        char buffer [maxSize];
        ssize_t recvReturnValue = ::recv (fd, &buffer, maxSize, 0); // flags = 0
        CInteropUtils::systemErrorCheck ("recv ()", recvReturnValue);
        return std::vector <char> (buffer, buffer + recvReturnValue);
    };
    void ClientSocket::write (std::vector <char> const & data) {
        const char* dataPointer (reinterpret_cast <const char*> (data.data ()));
        ssize_t sendReturnValue = send (fd, static_cast <const void*> (dataPointer), data.size (), 0); // flags = 0
        CInteropUtils::systemErrorCheck ("send ()", sendReturnValue);
    };
    ClientSocket::~ClientSocket () {
        int shutdownReturnValue = shutdown (fd, SHUT_RDWR);
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        int closeReturnValue = close (fd);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
    };
};

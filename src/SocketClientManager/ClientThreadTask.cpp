#include "ClientThreadTask.hpp"

namespace HTStack {
    ClientThreadTask::ClientThreadTask (int const & clientSocket_, sockaddr_in const & clientAddress_)
    : clientSocket (clientSocket_), clientAddress (clientAddress_);
};

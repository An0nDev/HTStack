#pragma once

namespace HTStack {
    class Server;
    class Request;
    class RequestReader {
    public:
        Server & server;
        RequestReader (Server & server_);
        Request readFrom (int const & sockfd);
    };
};

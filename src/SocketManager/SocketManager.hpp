#pragma once

#include <thread>
#include <sys/types.h>
#include <netinet/ip.h>
#include <system_error>
#include <mutex>
#include "../SocketClientManager/SocketClientManager.hpp"

namespace HTStack {
    class Server;
    class SocketManager {
    public:
        Server & server;
        bool isRunning;
        SocketManager (Server & server_);
        void start ();
        void shutdown ();
        ~SocketManager ();
    private:
        std::thread* runThread;

        sockaddr_in serverAddress;
        int serverSocket;

        void setup_ () throw (std::system_error);
        void run_ () throw (std::system_error);
        void cleanup_ () throw (std::system_error);

        static void system_error_check__ (std::string const & systemFunctionName, int & systemFunctionReturnValue) throw (std::system_error);
        friend SocketClientManager;

        SocketClientManager clientManager;
    };
};

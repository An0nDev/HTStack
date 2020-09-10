#pragma once

#include <netinet/ip.h>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace HTStack {
    class Server;
    class SocketClientManager;
    class SocketClientManager {
    private:
        class ThreadPoolTask {
        public:
            int clientSocket;
            sockaddr_in clientAddress;
            ThreadPoolTask (int const & clientSocket_, sockaddr_in const & clientAddress_);
        };
        class ThreadPoolTaskHolder {
        public:
            ThreadPoolTask* task = nullptr;
            bool isHolding;
            void hold (ThreadPoolTask task_);
            void drop ();
            ThreadPoolTaskHolder ();
            ~ThreadPoolTaskHolder ();
        };
        std::vector <std::thread*> clientThreadPool;
        int availableCount;
        std::mutex availableLock;
        std::condition_variable availableNotifier;
        ThreadPoolTask* newTask;
        std::condition_variable newTaskNotifier;
        std::mutex clientThreadLock;
        bool stopped;

        void closeConnection_ (int const & clientSocket);

        void clientThreadFunc ();
    public:
        explicit SocketClientManager (Server & server_);

        Server & server;

        void create (int const & clientSocket, sockaddr_in const & clientAddress);
        // NOTE: waitForAll does *not* disallow future calls to create;
        // make sure to disable the mechanism that calls create before calling waitForAll!
        void waitForAll ();
    };
};

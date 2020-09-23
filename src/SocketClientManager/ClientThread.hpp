#pragma once

#include "ClientThreadTask.hpp"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace HTStack {
    class Server;
    class ClientThread {
    private:
        Server & server;

        std::mutex canAcceptLock;
        bool canAccept_;
        std::mutex stoppedLock;
        bool stopped;
        void func ();
        void executeTask_ (ClientThreadTask const & task);

        std::mutex acceptanceTriggerLock;
        std::condition_variable acceptanceTrigger;

        std::mutex newTaskLock;
        ClientThreadTask* newTask;

        std::condition_variable & readyTrigger;

        std::thread* thread;
    public:
        ClientThread (Server & server_, std::condition_variable & readyTrigger_);
        bool canAccept ();
        void accept (ClientThreadTask const & task);
        ~ClientThread ();
    };
};

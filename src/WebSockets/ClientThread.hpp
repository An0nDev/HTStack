#pragma once

#include <mutex>

namespace HTStack::WebSockets {
    class ClientThread {
    private:
        Manager & manager;

        std::thread thread;
        void func ();

        std::mutex finishedLock;
        bool finished;
    public:
        ClientThread (Manager & manager_);
    };
};

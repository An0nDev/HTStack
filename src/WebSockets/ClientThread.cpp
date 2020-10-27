#include "ClientThread.hpp"

namespace HTStack::WebSockets {
    ClientThread::ClientThread (Manager & manager_) : manager (manager_) {};
    void ClientThread::func () {
        // do shit

        {
            std::lock_guard <std::mutex> finishedGuard (finishedLock);
            finished = true;
        }
        manager.cleanupOrShutdownEvent.set ();
    }
    ClientThread::~ClientThread () {

    };
};

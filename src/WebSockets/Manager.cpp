#include "Manager.hpp"

namespace HTStack::WebSockets {
    void Manager::cleanupFunc () {
        while (true) {
            cleanupOrShutdownEvent.wait ();

            {
                std::lock_guard <std::mutex> shuttingDownLockGuard (shuttingDownLock);
                if (shuttingDown) break;
            }

            {
                std::lock_guard <std::mutex> threadsLockGuard (threadsLock);
                std::vector <ClientThread*>::iterator cleanupTarget (threads.begin ());
                for (ClientThread* thread : threads) {
                    thread->finishedLock.unlock ();
                    if (thread->finished) {
                        thread->finishedLock.lock ();
                        delete thread;
                        threads.erase (cleanupTarget);
                        break;
                    }
                    thread->finishedLock.lock ();
                    cleanupTarget += 1;
                }
            }
        };
    };
    Manager::Manager (Server & server_) : server (server_) {};
    void Manager::start () {
        cleanupThread = new std::thread (&Manager::cleanupFunc, this);
    };
    void Manager::handle (WebSocket* const & webSocket) {
        std::lock_guard <std::mutex> threadsLockGuard (threadsLock);
        ClientThread* thread = new ClientThread (*this, webSocket);
        threads.push_back (thread);
    };
    void Manager::shutdown () {
        // trigger shit
        shuttingDownLock.lock ();
        shuttingDown = true;
        shuttingDownLock.unlock ();
        cleanupOrShutdownEvent.set ();
        cleanupThread->join ();
        delete cleanupThread;
    };
};

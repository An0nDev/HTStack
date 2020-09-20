#include "ClientThreadPool.hpp"

namespace HTStack {
    ClientThreadPool::ClientThreadPool () : filled (false) {};
    ClientThreadPool::fill (int const & size) {
        if (filled) drain ();
        for (int threadNumber = 0; threadNumber < size; threadNumber++) {
            ClientThread* newThread = new ClientThread ();
        }
        filled = true;
    };
    ClientThreadPool::execute (ClientThreadTask const & task) {
        bool executed = false;
        while (!executed) {
            for (ClientThread* thread : threads) {
                if (thread->canAccept ()) {
                    thread->accept (Task);
                    executed = true;
                    break;
                }
            }
            if (!executed) {
                std::unique_lock <std::mutex> readyTriggerUniqueLock;
                readyTrigger.wait (readyTriggerUniqueLock);
            }
        }
    };
    ClientThreadPool::drain () {
        {
            bool threadsStillRunning = true;
            while (threads.size () > 0) {
                threads.erase (std::remove_if)
                if (threads.size () == 0) return;
                {
                    std::unique_lock <std::mutex> readyTriggerUniqueLock;
                    readyTrigger.wait (readyTriggerUniqueLock);
                }
                for (ClientThread* thread : threads) {
                    if (thread->canAccept ()) {
                        delete thread;
                    }
                }
            }
        }
        threads.clear ();
        filled = false;
    };
    ClientThreadPool::~ClientThreadPool () {
        if (filled) drain ();
    };
}

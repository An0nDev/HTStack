#include "ClientThreadPool.hpp"
#include <stdexcept>

namespace HTStack {
    ClientThreadPool::ClientThreadPool (Server & server_) : server (server_), filled (false) {};
    void ClientThreadPool::fill (int const & size) {
        if (filled) drain ();
        for (int threadNumber = 0; threadNumber < size; threadNumber++) {
            ClientThread* newThread = new ClientThread (server, readyTrigger);
            threads.push_back (newThread);
        }
        filled = true;
    };
    void ClientThreadPool::execute (ClientThreadTask const & task) {
        if (!filled) throw std::logic_error ("Trying to execute task on unfilled pool");
        bool executed = false;
        while (!executed) {
            for (ClientThread* thread : threads) {
                if (thread->canAccept ()) {
                    thread->accept (task);
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
    void ClientThreadPool::drain () {
        // Don't know why I wrote this... ~ClientThread should take care of it
        /*
        {
            bool threadsStillRunning = true;
            while (threads.size () > 0) {
                threads.erase (std::remove_if (thread => ))
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
        */
        if (!filled) throw std::logic_error ("Draining empty pool");
        threads.clear ();
        filled = false;
    };
    ClientThreadPool::~ClientThreadPool () {
        if (filled) drain ();
    };
}

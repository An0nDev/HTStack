#include "ClientThread.hpp"

namespace HTStack {
    void ClientThread::func () {
        while (true) {
            readyTrigger.notify_one ();

            acceptanceTriggerLock.unlock ();
            acceptanceTrigger.wait ();
            acceptanceTriggerLock.lock ();

            stoppedLock.lock ();
            if (stopped) {
                stoppedLock.unlock ();
                break;
            }
            stoppedLock.unlock ();

            canAcceptLock.lock ();
            canAccept = false;
            canAcceptLock.unlock ();

            newTaskLock.lock ();
            ClientThreadTask task (*newTask);
            delete newTask;
            newTaskLock.unlock ();

            std::cout << "WE GOT A TASK!!!!!" << std::endl;

            canAcceptLock.lock ();
            canAccept = true;
            canAcceptLock.unlock ();

            readyTrigger.notify_one ();
        }
    };
    void ClientThread::executeTask_ (ClientThreadTask const & task) {

    };
    ClientThread::ClientThread (std::condition_variable & readyTrigger_) : canAccept_ (true), stopped (false), readyTrigger (readyTrigger_), thread (
        new std::thread (&ClientThread::func, *this)
    ) {};
    void ClientThread::canAccept () {
        canAcceptLock.lock ();
        return canAccept_;
        canAcceptLock.unlock ();
    };
    void ClientThread::accept (ClientThreadTask const & task) {
        newTaskLock.lock ();
        newTask = new ClientThreadTask (task);
        newTaskLock.unlock ();
        acceptanceTrigger.notify_one ();
    };
    ClientThread::~ClientThread () {
        stoppedLock.lock ();
        stopped = true;
        stoppedLock.unlock ();
        acceptanceTrigger.notify_one ();
        thread->join ();
        delete thread;
    };
};

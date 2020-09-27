#include "ClientThread.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "../Server/Server.hpp"
#include <iostream>
#include <optional>
#include <unistd.h>

namespace HTStack {
    void ClientThread::func () {
        while (true) {
            readyTrigger.notify_one ();

            {
                std::unique_lock <std::mutex> acceptanceTriggerUniqueLock (acceptanceTriggerLock);
                acceptanceTrigger.wait (acceptanceTriggerUniqueLock);
            }

            stoppedLock.lock ();
            if (stopped) {
                stoppedLock.unlock ();
                break;
            }
            stoppedLock.unlock ();

            canAcceptLock.lock ();
            canAccept_ = false;
            canAcceptLock.unlock ();

            newTaskLock.lock ();
            ClientThreadTask task (*newTask);
            delete newTask;
            newTaskLock.unlock ();

            executeTask_ (task);

            canAcceptLock.lock ();
            canAccept_ = true;
            canAcceptLock.unlock ();

            readyTrigger.notify_one ();
        }
    };
    void ClientThread::executeTask_ (ClientThreadTask const & task) {
        std::optional <Request> requestOptional = server.requestReader.readFrom (task.clientSocket);
        // bool closeConnection;
        if (requestOptional.has_value ()) {
            Request request = requestOptional.value ();
            server.appLoader.handleRequest (request);
            // closeConnection = ! (request.headers.count (std::string ("Connection")) > 0 && request.headers ["Connection"] == std::string ("keep-alive"));
        }

        delete task.clientSocket;
    };
    ClientThread::ClientThread (Server & server_, std::condition_variable & readyTrigger_) : server (server_), canAccept_ (true), stopped (false), readyTrigger (readyTrigger_), thread (
        new std::thread (&ClientThread::func, this)
    ) {};
    bool ClientThread::canAccept () {
        canAcceptLock.lock ();
        bool temp = canAccept_;
        canAcceptLock.unlock ();
        return temp;
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

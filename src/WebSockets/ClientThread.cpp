#include "ClientThread.hpp"
#include "Manager.hpp"

#include <iostream>

namespace HTStack::WebSockets {
    ClientThread::ClientThread (Manager & manager_, WebSocket* const & webSocket)
    : manager (manager_), thread (&ClientThread::func, this, webSocket) {};
    void ClientThread::func (WebSocket* const & webSocket) {
        // Actual stuff for the client thread to do goes here
        run_ (webSocket);
        cleanup_ ();
    };
    void ClientThread::run_ (WebSocket* const & webSocket) {
        std::cout << "message receive loop should go here" << std::endl;
        while (true) {
            try {
                DataFrame dataFrame = webSocket->recv_ ();
                // Handle data frame
            } catch (std::exception const & exception) {
                std::cout << "Caught exception when receiving from WebSocket: " << exception.what () << std::endl;
                webSocket->close ();
            }
            if (webSocket->closed) {
                break;
            }
        }
        delete webSocket;
    };
    void ClientThread::cleanup_ () {
        {
            std::lock_guard <std::mutex> finishedGuard (finishedLock);
            finished = true; // Marks this thread as finished
        }

        manager.cleanupOrShutdownEvent.set ();

        // Once the event is set, the manager will iterate over all threads,
        // looking for the thread that's finished, and call its destructor.
        // This is the best way I know of to clean up a thread since it obviously can't
        // clean itself up.
    };
    ClientThread::~ClientThread () {
        thread.join ();
    };
};

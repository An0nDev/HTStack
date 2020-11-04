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
        DataFrame::Data::Type messageType;
        std::vector <DataFrame> fragments;
        while (true) {
            try {
                fetchData_ ();
                DataFrame dataFrame = webSocket->recv_ ();
                if (dataFrame.isControl ()) {
                    handleControlFrame_ (dataFrame);
                } else {
                    if (!dataFrame.fin) {
                        // The current data frame is not the last fragment
                        if (fragments.size () == 0) {
                            // Since this is the first fragment, interpret its opcode as the message type
                            messageType = dataFrame.
                        }
                    } else {
                        // The current data frame is the last fragment
                    }
                }
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
    void ClientThread::fetchData_ () {

    };
    void ClientThread::handleControlFrame_ () {

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

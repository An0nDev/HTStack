#include "SocketClientManager.hpp"
#include "ClientThreadTask.hpp"
#include "../Server/Server.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../Request/Request.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include <unistd.h>
#include <optional>
#include <iostream>
#include <sstream>

namespace HTStack {
    /*
    SocketClientManager::ThreadPoolTask::ThreadPoolTask (int const & clientSocket_, sockaddr_in const & clientAddress_)
    : clientSocket (clientSocket_), clientAddress (clientAddress_) {};
    SocketClientManager::ThreadPoolTaskHolder::ThreadPoolTaskHolder () {
        task = nullptr;
        isHolding = false;
    }
    void SocketClientManager::ThreadPoolTaskHolder::hold (ThreadPoolTask task_) {
        task = new ThreadPoolTask (task_);
        isHolding = true;
    };
    void SocketClientManager::ThreadPoolTaskHolder::drop () {
        delete task;
        isHolding = false;
    };
    SocketClientManager::ThreadPoolTaskHolder::~ThreadPoolTaskHolder () {
        if (isHolding) {
            drop ();
        }
    }
    */

    SocketClientManager::SocketClientManager (Server & server_)
    : server (server_), pool (server) {
        /*
        for (int clientThreadNumber = 0; clientThreadNumber < server.configuration.clientThreadPoolSize; clientThreadNumber++) {
            std::thread* clientThread = new std::thread (&SocketClientManager::clientThreadFunc, this);
            clientThreadPool.push_back (clientThread);
        }
        availableCount = server.configuration.clientThreadPoolSize;
        */
        pool.fill (server.configuration.clientThreadPoolSize);
        stopped = false;
    };
    /*
    void SocketClientManager::safePrint_ (std::string const & message) {
        printLock.lock ();
        std::cout << std::this_thread::get_id () << ": " << message << std::endl;
        printLock.unlock ();
    };
    void SocketClientManager::clientThreadFunc () {
        while (true) {
            safePrint_ ("top of loop");
            if (stopped) {
                break; // This thread was still finishing while the shutdown was initiated
            }
            ThreadPoolTaskHolder taskHolder; // Create an outer-scope variable to hold the task

            {
                safePrint_ ("creating clientThreadUniqueLock");
                std::unique_lock <std::mutex> clientThreadUniqueLock (clientThreadLock);
                safePrint_ ("created, waiting for task");
                newTaskNotifier.wait (clientThreadUniqueLock);
                safePrint_ ("done waiting");
            }
            clientThreadLock.lock ();
            if (stopped) {
                break; // waitForAll was called
            }
            taskHolder.hold (*newTask); // Insert the task into the holder
            delete newTask; // Delete the object-scope one

            availableLock.lock ();
            availableCount--; // This thread is now in use
            availableLock.unlock ();

            clientThreadLock.unlock ();



            ThreadPoolTask task (*taskHolder.task); // Copy the held task into this scope
            taskHolder.drop ();
            std::optional <Request> requestOptional = server.requestReader.readFrom (task.clientSocket, task.clientAddress);
            bool closeConnection;
            if (requestOptional.has_value ()) {
                Request request = requestOptional.value ();
                server.appLoader.handleRequest (request);
                // closeConnection = ! (request.headers.count (std::string ("Connection")) > 0 && request.headers ["Connection"] == std::string ("keep-alive"));
            }

            closeConnection_ (task.clientSocket);

            availableLock.lock ();
            availableCount++; // This thread is no longer in use
            availableNotifier.notify_one ();
            availableLock.unlock ();
            safePrint_ ("bottom of loop");
        }
        safePrint_ ("thread exiting");
    };
    void SocketClientManager::closeConnection_ (int const & clientSocket) {
        int shutdownReturnValue = shutdown (clientSocket, SHUT_RDWR);
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        int closeReturnValue = close (clientSocket);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
    };
    */
    void SocketClientManager::create (int const & clientSocket, sockaddr_in const & clientAddress) {
        /*
        safePrint_ ("SocketClientManager::create () called");
        if (stopped) {
            // Closed client manager; this is an anomaly: close the socket and return.
            closeConnection_ (clientSocket);
            return;
        }

        safePrint_ ("getting access to availableCount");
        // Gain access to the count of available threads in the pool
        safePrint_ ("unique_lock created");
        safePrint_ ("locking availableLock");
        availableLock.lock ();
        safePrint_ ("availableLock locked");
        if (availableCount <= 0) {
            safePrint_ ("checked value (need to wait), unlocking availableLock");
            availableLock.unlock ();
            safePrint_ ("unlocked availableLock");
            {
                std::unique_lock <std::mutex> availableUniqueLock (availableLock);
                availableNotifier.wait (availableUniqueLock);
            }
        } else {
            safePrint_ ("checked value (don't need to wait), unlocking availableLock");
            availableLock.unlock ();
            safePrint_ ("unlocked availableLock");
        }

        safePrint_ ("locking clientThreadLock");
        clientThreadLock.lock ();
        safePrint_ ("locked clientThreadLock, creating task");
        newTask = new ThreadPoolTask (clientSocket, clientAddress);
        safePrint_ ("task created, unlocking clientThreadLock");
        clientThreadLock.unlock ();
        safePrint_ ("unlocked clientThreadLock");

        safePrint_ ("notifying one");
        newTaskNotifier.notify_one ();
        safePrint_ ("notified one");
        */

        pool.execute (ClientThreadTask (clientSocket, clientAddress));
    };

    void SocketClientManager::waitForAll () {
        /*
        clientThreadLock.lock ();

        stopped = true;
        safePrint_ ("notifying all");
        newTaskNotifier.notify_all ();
        safePrint_ ("notified all");

        for (std::thread* clientThread : clientThreadPool) {
            std::ostringstream ss;
            ss << clientThread->get_id ();
            safePrint_ ("waiting for client thread " + ss.str ());
            clientThread->join ();
            delete clientThread;
            safePrint_ ("cleaned up client thread" + ss.str ());
        };
        clientThreadPool.clear ();

        clientThreadLock.unlock ();
        */

        pool.drain ();
    };
};

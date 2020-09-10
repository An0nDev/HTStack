#include "SocketClientManager.hpp"
#include "../Server/Server.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "../RequestReader/RequestReader.hpp"
#include "../Request/Request.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include <unistd.h>
#include <optional>
#include <iostream>

namespace HTStack {
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

    SocketClientManager::SocketClientManager (Server & server_)
    : server (server_) {
        for (int clientThreadNumber = 0; clientThreadNumber < server.configuration.clientThreadPoolSize; clientThreadNumber++) {
            std::thread* clientThread = new std::thread (&SocketClientManager::clientThreadFunc, this);
            clientThreadPool.push_back (clientThread);
        }
        availableCount = server.configuration.clientThreadPoolSize;
        stopped = false;
    };
    void SocketClientManager::clientThreadFunc () {
        while (true) {
            ThreadPoolTaskHolder taskHolder; // Create an outer-scope variable to hold the task
            {
                std::unique_lock <std::mutex> clientThreadUniqueLock (clientThreadLock);
                newTaskNotifier.wait (clientThreadUniqueLock);
                if (stopped) {
                    break; // waitForAll was called
                }
                taskHolder.hold (*newTask); // Insert the task into the holder
                delete newTask; // Delete the object-scope one

                std::unique_lock <std::mutex> availableUniqueLock (availableLock);
                availableUniqueLock.unlock ();
                availableCount--; // This thread is now in use
                availableUniqueLock.lock ();
            }
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

            std::unique_lock <std::mutex> availableUniqueLock (availableLock);
            availableUniqueLock.unlock ();
            availableCount++; // This thread is no longer in use
            availableNotifier.notify_one ();
            availableUniqueLock.lock ();
        }
    };
    void SocketClientManager::closeConnection_ (int const & clientSocket) {
        int shutdownReturnValue = shutdown (clientSocket, SHUT_RDWR);
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        int closeReturnValue = close (clientSocket);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
    };

    void SocketClientManager::create (int const & clientSocket, sockaddr_in const & clientAddress) {
        if (stopped) {
            // Closed client manager; this is an anomaly: close the socket and return.
            closeConnection_ (clientSocket);
            return;
        }

        // Gain access to the count of available threads in the pool
        std::unique_lock <std::mutex> availableUniqueLock (availableLock);
        availableUniqueLock.unlock ();
        if (availableCount <= 0) {
            availableNotifier.wait (availableUniqueLock);
            availableUniqueLock.lock ();
        } else {
            availableUniqueLock.lock ();
        }

        std::unique_lock <std::mutex> clientThreadUniqueLock (clientThreadLock);
        clientThreadUniqueLock.unlock ();
        newTask = new ThreadPoolTask (clientSocket, clientAddress);
        clientThreadUniqueLock.lock ();

        newTaskNotifier.notify_one ();
    };

    void SocketClientManager::waitForAll () {
        clientThreadLock.unlock ();

        stopped = true;
        newTaskNotifier.notify_all ();

        for (std::thread* clientThread : clientThreadPool) {
            clientThread->join ();
            delete clientThread;
        };
        clientThreadPool.clear ();

        clientThreadLock.lock ();
    };
};

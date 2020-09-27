#include "SocketManager.hpp"

#include "../ClientSocket/ClientSocket.hpp"
#include "../ClientSocket/SSLSocket.hpp"

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../CInteropUtils/CInteropUtils.hpp"
#include <errno.h>
#include "../Server/Server.hpp"
#include "../SocketClientManager/SocketClientManager.hpp"
#include <iostream>

namespace HTStack {
    SocketManager::SocketManager (Server & server_)
    : server (server_), isRunning (false), clientManager (server), runThread (nullptr), serverSocket (-1), sslServer (nullptr) {
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
    };
    void SocketManager::start () {
        setup_ ();
        runThread = new std::thread (&SocketManager::run_, this);
        isRunning = true;
    };
    void SocketManager::shutdown () {
        cleanup_ ();
        isRunning = false;
        runThread->join ();
        delete runThread;
    };
    void SocketManager::setup_ () {
        if (server.configuration.ssl.enabled) {
            SSL_library_init ();
            sslServer = SSL_CTX_new (server.configuration.ssl.method.value ());
            if (sslServer == nullptr) {
                ERR_print_errors_fp (stderr); // Print errors to standard error
                throw std::runtime_error ("Unable to initialize SSL context");
            }
            if (SSL_CTX_use_certificate_chain_file (sslServer, server.configuration.ssl.certChainFilePath.value ().c_str ()) != 1) {
                ERR_print_errors_fp (stderr);
                throw std::runtime_error ("Unable to load certificate chain file into SSL context");
            }
            if (SSL_CTX_use_PrivateKey_file (sslServer, server.configuration.ssl.keyFilePath.value ().c_str (), server.configuration.ssl.keyFileType.value ()) != 1) {
                ERR_print_errors_fp (stderr);
                throw std::runtime_error ("Unable to load private key file into SSL context");
            }
        }

        serverSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
        CInteropUtils::systemErrorCheck ("socket ()", serverSocket);

        if (server.configuration.sloppySocketRestart) {
            int reuseAddrValue = 1; // non-zero value for enable since it's a boolean
            int setsockoptReturnValue = setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddrValue, sizeof (reuseAddrValue));
            CInteropUtils::systemErrorCheck ("setsockopt ()", setsockoptReturnValue);
        }

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr (server.configuration.ip.c_str ());
        serverAddress.sin_port = htons (server.configuration.port);
        unsigned long serverAddressSize = sizeof (serverAddress);
        int clientSocket = bind (serverSocket, (sockaddr*) &serverAddress, serverAddressSize);
        CInteropUtils::systemErrorCheck ("bind ()", clientSocket);

        int listenReturnValue = listen (serverSocket, server.configuration.backlog);
        CInteropUtils::systemErrorCheck ("listen ()", listenReturnValue);
    };
    void SocketManager::run_ () {
        while (true) {
            sockaddr_in clientAddress;
            unsigned long clientAddressSize = sizeof (clientAddress);

            int clientSocketFileDescriptor = accept (serverSocket, (sockaddr*) &clientAddress, (socklen_t*) &clientAddressSize);
            if (clientSocketFileDescriptor < 0 && errno == EINVAL) {
                // Broken from cleanup_
                break;
            }
            CInteropUtils::systemErrorCheck ("accept ()", clientSocketFileDescriptor);

            ClientSocket* clientSocket;
            try {
                if (server.configuration.ssl.enabled) {
                    clientSocket = new SSLSocket (clientSocketFileDescriptor, clientAddress, sslServer);
                } else {
                    clientSocket = new ClientSocket (clientSocketFileDescriptor, clientAddress);
                }

                clientManager.create (clientSocket);
            } catch (std::runtime_error const & exception) {
                std::cerr << "Caught non-fatal runtime error: " << exception.what () << std::endl;
            }
        }
    };
    void SocketManager::cleanup_ () {
        int shutdownReturnValue = ::shutdown (serverSocket, SHUT_RD); // causes accept to fail with EINVAL; see https://stackoverflow.com/a/19239058/5037905
        CInteropUtils::systemErrorCheck ("shutdown ()", shutdownReturnValue);

        clientManager.waitForAll ();

        int closeReturnValue = close (serverSocket);
        CInteropUtils::systemErrorCheck ("close ()", closeReturnValue);
        if (server.configuration.ssl.enabled) {
            SSL_CTX_free (sslServer);
            OPENSSL_cleanup ();
        }
    };
    SocketManager::~SocketManager () {
        if (isRunning) shutdown ();
    };
};

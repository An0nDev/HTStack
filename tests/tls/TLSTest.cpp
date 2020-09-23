#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <iostream>
#include <vector>

int create_socket(int port) {
    std::cout << "+ Creating socket" << std::endl;
    int s;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror("- Unable to create socket");
	exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
	perror("- Unable to bind");
	exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
	perror("- Unable to listen");
	exit(EXIT_FAILURE);
    }

    std::cout << "+ Socket created with file descriptor " << s << std::endl;

    return s;
}

void init_openssl () {
    std::cout << "+ Initializing OpenSSL" << std::endl;
    SSL_load_error_strings ();
    OpenSSL_add_ssl_algorithms ();
    std::cout << "+ OpenSSL initialized" << std::endl;
}

void cleanup_openssl () {
    std::cout << "+ Cleaning up OpenSSL" << std::endl;
    EVP_cleanup ();
    std::cout << "+ Cleaned up OpenSSL" << std::endl;
}

SSL_CTX* create_context () {
    std::cout << "+ Creating OpenSSL context" << std::endl;
    const SSL_METHOD* method;
    SSL_CTX* ctx;

    method = SSLv23_server_method (); // Get dispatch table for the server calls for SSL v2 and v3

    ctx = SSL_CTX_new (method); // Create a new SSL context with this dispatch table
    if (!ctx) {
    	perror ("- Unable to create SSL context"); // Print this string and a description of errno to stderr
    	ERR_print_errors_fp (stderr); // Print SSL errors to stderr
    	exit (EXIT_FAILURE);
    }

    std::cout << "+ Created OpenSSL context" << std::endl;

    return ctx;
}

void configure_context (SSL_CTX *ctx) {
    std::cout << "+ Configuring OpenSSL context" << std::endl;
    SSL_CTX_set_ecdh_auto(ctx, 1); // Automatically selects the curve used for cryptography on the SSL context

    if (SSL_CTX_use_certificate_chain_file(ctx, "cert.pem") <= 0) { // Setting the certificate for the context
        std::cout << "- SSL_CTX_use_certificate_file failed" << std::endl;
        ERR_print_errors_fp(stderr); // Print SSL errors
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) { // Setting the private key for the context
        std::cout << "- SSL_CTX_use_PrivateKey_file failed" << std::endl;
        ERR_print_errors_fp(stderr); // Print SSL errors
        exit(EXIT_FAILURE);
    }
    std::cout << "+ Configured OpenSSL context" << std::endl;
}

static const char response [] = "HTTP/1.1 200 Gucci\r\n\r\nBruh"; // stupid HTTP response
static const unsigned int responseLength = strlen (response);

int main (int argc, char **argv) {
    std::cout << "+ Starting..." << std::endl;
    // Load OpenSSL and create and configure an SSL context
    init_openssl ();
    SSL_CTX* ctx = create_context ();
    configure_context (ctx);

    int serverSocket = create_socket (4445); // Create a server socket on port 4433
    int reuseAddrValue = 1; // non-zero value for enable since it's a boolean
    int setsockoptReturnValue = setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddrValue, sizeof (reuseAddrValue));
    if (setsockoptReturnValue < 0) {
        perror ("- Enabling SO_REUSEADDR failed");
        exit (EXIT_FAILURE);
    }

    /* Handle connections */
    while (true) {
        struct sockaddr_in addr;
        unsigned int len = sizeof (addr);
        int client = accept (serverSocket, (struct sockaddr*) &addr, &len); // Get a client socket
        if (client < 0) {
            perror ("- Unable to accept"); // Print error
            exit (EXIT_FAILURE);
        }

        SSL* ssl = SSL_new (ctx); // Initialize an SSL client object
        SSL_set_fd (ssl, client); // Set the file descriptor of the SSL client object to the client socket

        if (SSL_accept (ssl) <= 0) { // -1 = fatal error, 0 = controlled shutdown
            std::cout << "- SSL_accept failed" << std::endl;
            ERR_print_errors_fp (stderr); // Print error
        } else {
            std::cout << "+ Reading request" << std::endl;
            // stupid HTTP request reader, reads until first \r\n
            {
                char lastChar;
                while (true) {
                    char newChar;
                    SSL_read (ssl, &newChar, 1);
                    if (strncmp (&lastChar, "\r", 1) == 0 && strncmp (&newChar, "\n", 1) == 0) { // if last character is \r and this character is \n
                        break;
                    }
                    lastChar = newChar;
                }
            } // frees lastChar
            std::cout << "+ Read request, writing response" << std::endl;
            SSL_write (ssl, response, responseLength); // Write reply to SSL client
            std::cout << "+ Wrote response" << std::endl;
        }

        SSL_shutdown (ssl); // Shut down the SSL client object
        SSL_free (ssl); // Free the memory for the SSL client object
        close (client); // Close the client socket
    }

    close (serverSocket); // Close the server socket
    SSL_CTX_free (ctx); // Free the memory for the SSL context
    cleanup_openssl (); // Unload OpenSSL
    std::cout << "+ Done!" << std::endl;
}

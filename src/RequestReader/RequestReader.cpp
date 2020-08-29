#include "RequestReader.hpp"
#include <iostream>
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../Request/Request.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace HTStack {
    const std::string RequestReader::CRLF ("\r\n");
    std::optional <std::vector <char>> RequestReader::recv_ (int const & clientSocket) {
        char recvBuffer [server.configuration.maxRecvSize];
        ssize_t recvReturnValue = recv (clientSocket, &recvBuffer, server.configuration.maxRecvSize, 0); // no flags
        if (recvReturnValue == -1 && errno == EINVAL) {
            return std::nullopt; // Failure (socket closed)
        }
        CInteropUtils::systemErrorCheck ("recv ()", recvReturnValue);
        std::vector <char> dataBytes (recvBuffer, recvBuffer + recvReturnValue);
        return std::optional <std::vector <char>> {dataBytes}; // Success
    };
    std::optional <std::string> RequestReader::recvUntil_ (int const & clientSocket, std::string endPattern) {
        std::string completeString = leftoverFromLastRecvUntil; // copy leftover data from last call
        size_t patternPosition;
        while (true) {
            patternPosition = completeString.find (endPattern);
            if (patternPosition != std::string::npos) {
                break; // Match found
            }
            std::optional <std::vector <char>> recvResult = recv_ (clientSocket);
            if (!recvResult.has_value ()) {
                return std::nullopt; // Failure (socket closed)
            }
            std::string recvString (recvResult.value ().data (), recvResult.value ().size ()); // as in "string that was recv"
            completeString.append (recvString);
        }
        size_t patternEndPosition = patternPosition + endPattern.size ();
        leftoverFromLastRecvUntil.clear (); // Empty the member for leftover data
        if (patternEndPosition > (completeString.size () - 1)) {
            // We have leftover data, append to the member
            leftoverFromLastRecvUntil.append (completeString.substr (patternEndPosition, std::string::npos));
        }
        return std::optional <std::string> {completeString.substr (0, patternPosition)}; // return the complete string without the end pattern
    };
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    std::optional <Request> RequestReader::readFrom (int const & clientSocket, sockaddr_in const & clientAddress) {
        std::cout << "Reading from " << clientSocket << std::endl;
        std::optional <std::string> requestLineOptional = recvUntil_ (clientSocket, CRLF);
        if (!requestLineOptional.has_value ()) {
            return std::nullopt;
        }
        std::string requestLine = requestLineOptional.value ();
        std::cout << "Request line: " << requestLine << std::endl;
        return std::optional <Request> {Request ()};
    };
};

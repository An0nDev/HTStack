#include "InternalReader.hpp"
#include "../Server/Server.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include <iostream>

namespace HTStack {
    InternalReader::InternalReader (Server const & server_, int const & clientSocket_)
    : server (server_), clientSocket (clientSocket_) {};
    std::optional <std::vector <char>> InternalReader::recv () {
        char recvBuffer [server.configuration.maxRecvSize];
        ssize_t recvReturnValue = ::recv (clientSocket, &recvBuffer, server.configuration.maxRecvSize, 0); // no flags
        if (recvReturnValue == -1 && errno == EINVAL) {
            return std::nullopt; // Failure (socket closed)
        }
        CInteropUtils::systemErrorCheck ("recv ()", recvReturnValue);
        std::vector <char> dataBytes (recvBuffer, recvBuffer + recvReturnValue);
        return std::optional <std::vector <char>> {dataBytes}; // Success
    };
    std::optional <std::vector <char>> InternalReader::recvData (int const & length) {
        throw std::logic_error ("recvData () not yet implemented");
        return std::nullopt; // make the compiler happy
    };
    std::optional <std::string> InternalReader::recvTextUntil (std::string const & endPattern) {
        std::string completeString (leftoverFromLastCall.begin (), leftoverFromLastCall.end ()); // copy leftover data from last call
        // ^ https://stackoverflow.com/q/8247793/5037905
        size_t patternPosition;
        while (true) {
            patternPosition = completeString.find (endPattern);
            if (patternPosition != std::string::npos) {
                break; // Match found
            }
            std::optional <std::vector <char>> recvResult = recv ();
            if (!recvResult.has_value ()) {
                return std::nullopt; // Failure (socket closed)
            }
            std::string recvString (recvResult.value ().data (), recvResult.value ().size ()); // as in "string that was recv"
            completeString.append (recvString);
        }
        size_t patternEndPosition = patternPosition + endPattern.size ();
        leftoverFromLastCall.clear (); // Empty the member for leftover data
        if (patternEndPosition < (completeString.size () - 1)) {
            // We have leftover data, append to the member
            std::string leftover = completeString.substr (patternEndPosition, std::string::npos);
            std::vector <char> leftoverAsBinaryData (leftover.begin (), leftover.end ());
            leftoverFromLastCall.insert (leftoverFromLastCall.end (), leftoverAsBinaryData.begin (), leftoverAsBinaryData.end ()); // https://stackoverflow.com/a/201729/5037905
        }
        return std::optional <std::string> {completeString.substr (0, patternPosition)}; // return the complete string without the end pattern
    };
    void InternalReader::ensureEmpty () {
        if (leftoverFromLastCall.size () > 0) {
            throw std::logic_error ("Leftover data in leftoverFromLastCall!");
        }
    }
};

#include "RequestReader.hpp"

#include "InternalReader.hpp"
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"

#include <iostream>
#include <map>
#include <utility>
#include <stdexcept>

namespace HTStack {
    const std::string RequestReader::CRLF ("\r\n");
    const std::string RequestReader::headerNameAndValueSeparator (": ");
    const std::string RequestReader::methodAndPathSeparator (" ");
    const std::string RequestReader::pathAndVersionSeparator (" ");
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    Request RequestReader::readFrom (ClientSocket* const & clientSocket) {
        InternalReader reader (server, clientSocket);

        std::string requestLine = reader.recvTextUntil (CRLF);
        size_t methodAndPathSeparatorLocation = requestLine.find (methodAndPathSeparator);
        if (methodAndPathSeparatorLocation == std::string::npos) {
            throw std::runtime_error ("Method and path separator not found");
        }
        std::string methodString (
            requestLine.substr (0, methodAndPathSeparatorLocation)
        );
        Request::Method method = Request::methodStringToEnum (methodString);
        std::string pathAndVersion (
            requestLine.substr (
                methodAndPathSeparatorLocation + methodAndPathSeparator.size (),
                requestLine.size () - (
                    methodAndPathSeparatorLocation + methodAndPathSeparator.size ()
                )
            )
        );
        size_t pathAndVersionSeparatorLocation = pathAndVersion.find (pathAndVersionSeparator);
        if (pathAndVersionSeparatorLocation == std::string::npos) {
            throw std::runtime_error ("Path and version separator not found");
        }
        std::string path (
            pathAndVersion.substr (0, pathAndVersionSeparatorLocation)
        );
        // TODO URL unescape
        std::string version (
            pathAndVersion.substr (
                pathAndVersionSeparatorLocation + pathAndVersionSeparator.size (),
                pathAndVersion.size () - (
                    pathAndVersionSeparatorLocation + pathAndVersionSeparator.size ()
                )
            )
        );

        std::map <std::string, std::string> headers;
        while (true) {
            std::string headerLine = reader.recvTextUntil (CRLF);
            if (headerLine.empty ()) {
                break;
            }
            size_t nameAndValueSeparatorLocation = headerLine.find (headerNameAndValueSeparator);
            if (nameAndValueSeparatorLocation == std::string::npos) {
                continue;
            }
            std::string headerName (
                headerLine.substr (0, nameAndValueSeparatorLocation)
            );
            std::string headerValue (
                headerLine.substr (
                    nameAndValueSeparatorLocation + headerNameAndValueSeparator.size (),
                    headerLine.size () - (
                        nameAndValueSeparatorLocation + headerNameAndValueSeparator.size ()
                    )
                )
            );
            headers.insert ({headerName, headerValue});
        }
        return Request (server, clientSocket, method, path, headers);
    };
};

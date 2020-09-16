#include "RequestReader.hpp"
#include <iostream>
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "InternalReader.hpp"
#include <map>
#include <utility>

namespace HTStack {
    const std::string RequestReader::CRLF ("\r\n");
    const std::string RequestReader::headerNameAndValueSeparator (": ");
    const std::string RequestReader::methodAndPathSeparator (" ");
    const std::string RequestReader::pathAndVersionSeparator (" ");
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    std::optional <Request> RequestReader::readFrom (int const & clientSocket, sockaddr_in const & clientAddress) {
        InternalReader reader (server, clientSocket);

        std::optional <std::string> requestLineOptional = reader.recvTextUntil (CRLF);
        if (!requestLineOptional.has_value ()) {
            return std::nullopt;
        }
        std::string requestLine = requestLineOptional.value ();
        size_t methodAndPathSeparatorLocation = requestLine.find (methodAndPathSeparator);
        if (methodAndPathSeparatorLocation == std::string::npos) {
            return std::nullopt;
        }
        std::string methodString (
            requestLine.substr (0, methodAndPathSeparatorLocation)
        );
        std::optional <Request::Method> methodOptional = Request::methodStringToEnum (methodString);
        if (!methodOptional.has_value ()) {
            return std::nullopt; // Invalid method string
        }
        Request::Method method = methodOptional.value ();
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
            return std::nullopt;
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
            std::optional <std::string> headerLineOptional = reader.recvTextUntil (CRLF);
            if (!headerLineOptional.has_value ()) {
                return std::nullopt;
            }
            std::string headerLine = headerLineOptional.value ();
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
        reader.ensureEmpty ();
        return std::optional <Request> {Request (server, clientSocket, method, path, headers)};
    };
};

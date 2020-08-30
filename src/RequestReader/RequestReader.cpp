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
    RequestReader::RequestReader (Server & server_)
    : server (server_) {};
    std::optional <Request> RequestReader::readFrom (int const & clientSocket, sockaddr_in const & clientAddress) {
        std::cout << "Reading from " << clientSocket << std::endl;

        InternalReader reader (server, clientSocket);

        std::optional <std::string> requestLineOptional = reader.recvTextUntil (CRLF);
        if (!requestLineOptional.has_value ()) {
            return std::nullopt;
        }
        std::string requestLine = requestLineOptional.value ();
        std::cout << "Request line: " << requestLine << std::endl;

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
        std::cout << "Header count: " << headers.size () << std::endl;
        for (std::pair <std::string, std::string> header : headers) {
            std::cout << "Header " << header.first << " has value '" << header.second << "'" << std::endl;
        }

        return std::optional <Request> {Request ()};
    };
};

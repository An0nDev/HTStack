#include "RequestReader.hpp"
#include <iostream>
#include "../Server/Server.hpp"
#include "../ServerConfiguration/ServerConfiguration.hpp"
#include "../CInteropUtils/CInteropUtils.hpp"
#include "InternalReader.hpp"

namespace HTStack {
    const std::string RequestReader::CRLF ("\r\n");
    const std::string RequestReader::headerNameAndValueSeparator (": ");
    std::vector <Request::Header> RequestReader::parseHeaderString (std::string headerString) {
        std::vector <Request::Header> headers;
        size_t crlfLocation = 0;
        while ((crlfLocation = headerString.find (CRLF, crlfLocation)) != std::string::npos) {
            std::string headerLine = headerString.substr (0, crlfLocation);
            std::cout << "Header line: '" << headerLine << "'" << std::endl;
            size_t colonSpaceLocation = headerString.find (headerNameAndValueSeparator);
            if (colonSpaceLocation == std::string::npos) {
                continue;
            }
            std::string headerName (headerLine.substr (0, colonSpaceLocation));
            std::string headerValue (headerLine.substr (colonSpaceLocation + headerNameAndValueSeparator.size (), headerLine.size () - (colonSpaceLocation + headerNameAndValueSeparator.size ())));
            Request::Header header (headerName, headerValue);
            headers.push_back (header);
            headerString = headerString.substr (crlfLocation + CRLF.size (), headerString.size () - (crlfLocation + CRLF.size ()));
        };
        return headers;
    };
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

        std::vector <Request::Header> headers;
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
            Request::Header header (headerName, headerValue);
            headers.push_back (header);
        }
        std::cout << "Header count: " << headers.size () << std::endl;
        for (Request::Header header : headers) {
            std::cout << "Header " << header.name << " has value '" << header.value << "'" << std::endl;
        }

        return std::optional <Request> {Request ()};
    };
};

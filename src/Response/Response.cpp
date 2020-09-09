#include "Response.hpp"
#include <iostream>
#include <stdexcept>
#include "../CInteropUtils/CInteropUtils.hpp"
#include <sys/types.h>
#include <sys/socket.h>

namespace HTStack {
    const std::string Response::CRLF ("\r\n");
    const std::string Response::versionString ("HTTP/1.1");
    const std::string Response::versionAndStatusSeparator (" ");
    const std::string Response::statusCodeAndTextSeparator (" ");
    const std::string Response::headerNameAndValueSeparator (": ");
    const std::map <int, std::string> Response::statuses {
        // Information responses
        {100, "Continue"},
        {101, "Switching Protocol"},
        {102, "Processing"},
        {103, "Early Hints"},

        // Successful responses
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {207, "Multi-Status"},
        {208, "Already Reported"},
        {226, "IM Used"},

        // Redirection messages
        {300, "Multiple Choice"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {306, "unused"},
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},

        // Client error responses
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Entity"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},

        // Server error responses
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"}
    };
    void Response::sendTo (int const & clientSocket) {
        if (statuses.count (statusCode) == 0) {
            throw std::logic_error ("Invalid response status code " + std::to_string (statusCode));
        }
        std::string startLine;
        startLine.append (versionString);
        startLine.append (versionAndStatusSeparator);
        startLine.append (std::to_string (statusCode));
        startLine.append (statusCodeAndTextSeparator);
        startLine.append (statuses.at (statusCode));
        startLine.append (CRLF);
        writeText_ (clientSocket, startLine);
        headers.try_emplace ("Content-Length", std::to_string (data.size ()));
        for (std::pair <std::string, std::string> header : headers) {
            std::string headerString (header.first + headerNameAndValueSeparator + header.second + CRLF);
            writeText_ (clientSocket, headerString);
        }
        writeText_ (clientSocket, CRLF);
        writeData_ (clientSocket, data);
    };
    void Response::writeText_ (int const & clientSocket, std::string const & text) {
        std::vector <char> textVector (text.begin (), text.end ());
        writeData_ (clientSocket, textVector);
    };
    void Response::writeData_ (int const & clientSocket, std::vector <char> const & data) {
        const char* dataPointer (reinterpret_cast <const char*> (data.data ()));
        ssize_t sendReturnValue = send (clientSocket, static_cast <const void*> (dataPointer), data.size (), 0);
    }
    Response::Response (int const & statusCode_)
    : statusCode (statusCode_) {};
    Response::Response (int const & statusCode_, std::map <std::string, std::string> const & headers_)
    : statusCode (statusCode_), headers (headers_) {};
    Response::Response (int const & statusCode_, std::string const & text)
    : statusCode (statusCode_), data (text.begin (), text.end ()) {}
    Response::Response (int const & statusCode_, std::vector <char> const & data_)
    : statusCode (statusCode_), data (data_) {};
    void Response::respondTo (Request const & request) {
        sendTo (request.clientSocket);
    };
};

#pragma once

#include "../Request/Request.hpp"
#include "../HTTPUtils/MIMEType.hpp"

#include <map>
#include <string>
#include <vector>
#include <optional>

namespace HTStack {
    class Response {
    private:
        static const std::string CRLF;
        static const std::string versionString;
        static const std::string versionAndStatusSeparator;
        static const std::string statusCodeAndTextSeparator;
        static const std::string headerNameAndValueSeparator;
        static const std::map <int, std::string> statuses;
        void sendTo (ClientSocket* const & clientSocket, int const & streamedResponseBufferSize);
        void writeText_ (ClientSocket* const & clientSocket, std::string const & text);
    public:
        int statusCode;
        std::map <std::string, std::string> headers;
        bool hasData;
        std::vector <char> data;
        bool streamed;
        std::istream* inputStream;
        bool hasMimeType;
        bool ownsMimeType;
        MIMEType* mimeType;
        explicit Response (int const & statusCode_);
        explicit Response (int const & statusCode_, std::map <std::string, std::string> const & headers_);
        explicit Response (int const & statusCode_, std::string const & text);
        explicit Response (int const & statusCode_, std::vector <char> const & data_);
        explicit Response (int const & statusCode_, std::istream* inputStream_, MIMEType* mimeType_);
        void respondTo (Request const & request);
        ~Response ();
    };
};

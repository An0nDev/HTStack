#include <map>
#include <string>
#include <vector>
#include "../Request/Request.hpp"

namespace HTStack {
    class Response {
    private:
        static const std::string CRLF;
        static const std::string versionString;
        static const std::string versionAndStatusSeparator;
        static const std::string statusCodeAndTextSeparator;
        static const std::map <int, std::string> statuses;
        void sendTo (int const & clientSocket);
        void writeText_ (int const & clientSocket, std::string const & text);
    public:
        int statusCode;
        std::map <std::string, std::string> headers;
        std::vector <char> data;
        Response (int const & statusCode_);
        Response (int const & statusCode_, std::map <std::string, std::string> const & headers_);
        Response (int const & statusCode_, std::vector <char> const & data_);
        void respondTo (Request const & request);
    };
};

#include <map>
#include <string>
#include <vector>
#include "../Request/Request.hpp"

namespace HTStack {
    class Response {
    private:
        void sendTo (int const & clientSocket);
    public:
        int responseCode;
        std::map <std::string, std::string> headers;
        std::vector <char> data;
        Response (int const & responseCode_);
        Response (int const & responseCode_, std::map <std::string, std::string> const & headers_);
        Response (int const & responseCode_, std::vector <char> const & data_);
        void respondTo (Request const & request);
    };
};

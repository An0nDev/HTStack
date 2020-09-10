#include <optional>
#include <vector>

namespace HTStack {
    class Server;
    class InternalReader {
    private:
        std::vector <char> leftoverFromLastCall;
        Server const & server;
        int const & clientSocket;
        std::optional <std::vector <char>> recv ();
    public:
        InternalReader (Server const & server_, int const & clientSocket_);
        std::optional <std::vector <char>> recvData (int const & length);
        std::optional <std::string> recvTextUntil (std::string const & endPattern);
        void ensureEmpty ();
    };
};

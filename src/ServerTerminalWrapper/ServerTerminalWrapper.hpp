#include <string>
#include <vector>

namespace HTStack {
    class Server;
    class ServerTerminalWrapper {
    private:
        static const std::string tokenDelimiter;

        Server & server;
        std::vector <std::string> history;
        void handleCommand (std::string const & command, std::vector <std::string> const & args);
    public:
        ServerTerminalWrapper (Server & server_);
        void prompt ();
    };
};

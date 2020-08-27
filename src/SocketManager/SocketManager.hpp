#include <thread>

namespace HTStack {
    class Server;
    class SocketManager;
    void SocketManagerRun_ (SocketManager* socketManager);
    class SocketManager {
        friend void SocketManagerRun_ (SocketManager* socketManager);
    private:
        std::thread* runThread;
        void run_ ();
    public:
        Server & server;
        bool isRunning;
        SocketManager (Server & server_);
        void start ();
        void shutdown ();
        ~SocketManager () throw (std::logic_error);
    };
};

#include "ClientThread.hpp"
#include <condition_variable>

namespace HTStack {
    class Server;
    class ClientThreadPool {
        Server & server;
        int size;
        bool filled;
        std::vector <ClientThread*> threads;

        std::mutex readyTriggerLock;
        std::condition_variable readyTrigger;
    public:
        ClientThreadPool (Server & server_);
        void fill (int const & size);
        void execute (ClientThreadTask const & task);
        void drain ();
        ~ClientThreadPool ();
    };
}

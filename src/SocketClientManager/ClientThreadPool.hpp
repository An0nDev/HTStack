#include "ClientThread.hpp"
#include <condition_variable>

namespace HTStack {
    class ClientThreadPool {
        int size;
        bool filled;
        std::vector <ClientThread*> threads;

        std::mutex readyTriggerLock;
        std::condition_variable readyTrigger;
    public:
        ClientThreadPool ();
        void fill (int const & size);
        void execute (ClientThreadTask const & task);
        void drain ();
        ~ClientThreadPool ();
    };
}

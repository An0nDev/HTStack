#include <vector>

namespace HTStack {
    class ClientSocket {
    private:
        int fd;
    public:
        bool keepOpen = false;
        ClientSocket (int const & fd_);
        virtual std::vector <char> read (int const & maxSize) = 0;
        virtual void write (std::vector <char> const & data) = 0;
        virtual ~ClientSocket ();
    };
};

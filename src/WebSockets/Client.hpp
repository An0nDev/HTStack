namespace HTStack::WebSockets {
    class Client {
    private:
        ClientSocket* underlyingClientSocket;
    public:
        Client (ClientSocket* const & underlyingClientSocket_);
    };
};

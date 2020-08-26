namespace HTStack {
    class App {
        HTStack::Server & server; // Reference to the server containing this app
    public:
        virtual void onStartup () = 0;
        virtual void onRequest (HTStack::Request & request) = 0;
        virtual void onTeardown () = 0;
        virtual ~HTStackApp () {};
    };
};

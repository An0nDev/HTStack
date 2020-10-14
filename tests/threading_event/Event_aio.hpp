#include <mutex>
#include <condition_variable>

class Event {
private:
    std::mutex isSetLock;
    bool isSet_ = false;
    std::mutex setTriggerLock;
    std::condition_variable setTrigger;
public:
    bool isSet () {
        isSetLock.lock ();
        bool isSetRet (isSet_);
        isSetLock.unlock ();
        return isSetRet;
    };
    void set () {
        isSetLock.lock ();
        isSet_ = true;
        isSetLock.unlock ();
        setTrigger.notify_all ();
    };
    void clear () {
        isSetLock.lock ();
        isSet_ = false;
        isSetLock.unlock ();
    };
    void wait () {
        if (!isSet ()) {
            std::unique_lock <std::mutex> setTriggerUniqueLock (setTriggerLock);
            setTrigger.wait (setTriggerUniqueLock);
        }
    };
};

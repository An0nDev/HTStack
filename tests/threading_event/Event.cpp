#include "Event.hpp"

bool Event::isSet () {
    isSetLock.lock ();
    bool isSetRet (isSet_);
    isSetLock.unlock ();
    return isSetRet;
};

void Event::set () {
    isSetLock.lock ();
    isSet_ = true;
    isSetLock.unlock ();
    setTrigger.notify_all ();
};

void Event::clear () {
    isSetLock.lock ();
    isSet_ = false;
    isSetLock.unlock ();
};

void Event::wait () {
    if (!isSet ()) {
        std::unique_lock <std::mutex> setTriggerUniqueLock (setTriggerLock);
        setTrigger.wait (setTriggerUniqueLock);
    }
};

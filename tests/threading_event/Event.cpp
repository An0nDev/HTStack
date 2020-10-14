#include "Event.hpp"

bool Event::isSet () {
    std::lock_guard <std::mutex> lockGuard (lock);
    return isSet_;
};

void Event::set () {
    std::lock_guard <std::mutex> lockGuard (lock);
    isSet_ = true;
    trigger.notify_all ();
};

void Event::clear () {
    std::lock_guard <std::mutex> lockGuard (lock);
    isSet_ = false;
};

void Event::wait () {
    while (!isSet ()) {
        std::unique_lock <std::mutex> uniqueLock (lock);
        trigger.wait (uniqueLock);
    }
};

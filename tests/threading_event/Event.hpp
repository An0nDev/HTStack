#pragma once

#include <mutex>
#include <condition_variable>

class Event {
private:
    std::mutex isSetLock;
    bool isSet_ = false;
    std::mutex setTriggerLock;
    std::condition_variable setTrigger;
public:
    bool isSet ();
    void set ();
    void clear ();
    void wait ();
};

#include "Test.hpp"
#include <thread>
#include <iostream>

Test::Test () {
    std::thread thread (&Test::thread_, this);
    event.set ();
    thread.join ();
};
void Test::thread_ () {
    event.wait ();
};

int main () {
    Test test;
};

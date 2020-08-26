#include <iostream>
#include "ToBeLoaded.hpp"

void ToBeLoaded::callableFunction () {
    std::cout << "It worked!" << std::endl;
};

ToBeLoaded::~ToBeLoaded () {}; // nothing to destruct

extern "C" LoadableClass* factory () {
    return static_cast <LoadableClass*> (new ToBeLoaded);
};

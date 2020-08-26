#include "Library.hpp"

int main () {
    Library library ("ToBeLoaded.so");
    LoadableClass* loadedClass = library.instantiate ();
    loadedClass->callableFunction ();
}

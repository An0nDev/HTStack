#include <dlfcn.h>
#include <iostream>
#include "Library.hpp"

Library::Library (std::string const & path) {
    handle = dlopen (path.data (), RTLD_NOW); // resolve all symbols immediately
    if (handle == nullptr) throw std::system_error ();
};

LoadableClass* Library::instantiate () {
    void* factoryVoidPointer = dlsym (handle, "factory"); // get pointer "factory" function which returns LoadableClass* from handle
    LoadableClassFactory factory = reinterpret_cast <LoadableClassFactory> (factoryVoidPointer);
    return factory ();
};

Library::~Library () {
    dlclose (handle);
};

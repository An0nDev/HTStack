#include <string>
#include "LoadableClass.hpp"

typedef void* LibraryHandle;
typedef LoadableClass* (*LoadableClassFactory) ();

class Library {
    LibraryHandle handle;
public:
    Library (std::string const & path);
    LoadableClass* instantiate ();
    ~Library ();
};

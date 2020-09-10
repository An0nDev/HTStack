#!/bin/bash
set -e # exit if non-zero return value from any command (not incl. pipes)
clang++ -fPIC -shared LoadableClass.cpp ToBeLoaded.cpp -o ToBeLoaded.so
clang++ -ldl LoadableClass.cpp Library.cpp Test.cpp -o Test
LD_LIBRARY_PATH=. ./Test
rm ToBeLoaded.so
rm Test

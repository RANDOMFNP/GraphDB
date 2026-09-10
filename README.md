# graphDB 🇬

GraphDB is a C++ library that empowers users to create and edit graphs!

If you want to contribute to GRAPHDB, check out the CONTRIBUTING.md file!

If this product helped you, please leave a star!
If it doesn't, tell me what's wrong, I respond to issues within 1-3 days!

# UPDATE COMING FRIDAY

### READEME.md UPDATES EVERY DAY AFTER A VERSION IS PUBLISHED

## NEWS

Switched license from AGPL v3 to Apache 2.0

## Great first issues

Here I will post some of the best first issues I see and its status

* https://github.com/RANDOMFNP/graphdb/issues/8 -- Bellman-Ford
* https://github.com/RANDOMFNP/graphdb/issues/9 -- A*
* https://github.com/RANDOMFNP/graphdb/issues/10 -- README.md

## Importing graphdb 📦

For most applications, `#include "graphdb.hpp"` will suffice.
If you are using a C++23 compiler that supports import, you can use `import graphdb;`


## Linking without CMake 📦

### Using g++

Example usage: `g++ program.cpp -I graphdb/include/ -I graphdb/detail/ -o program`, make sure to include
graphdb/include/ and graphdb/detail/ __NOTE:__ This is a C++ 23 library and will not function if you
are on too old of a g++ compiler, we recommend g++ 15 as a minimum

### Using clang

Example usage: `clang++ program.cpp -I graphdb/include/ -I ./graphdb/detail/ -o program`, make sure to include
graphdb/include/ and graphdb/detail/ __NOTE:__ This is C++ 23 library and will not function if you
are on too old of a clang compiler, we recommend clang 19 as a minimum

### Using MSVC

Example usage: `cl.exe /EHsc program.cpp /Fe:program.exe`
__NOTE:__ This is a C++ 23 library and will not function if you are using too old of an MVSC
compiler, we recommend MSVC 19.44 as a minimum

## PREREQUISITES

cmake --version == 3.30

### COMPILERS

g++ --version == >15

clang++ -version == >19

cl == > 19.50

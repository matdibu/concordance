# concordance
[![build](https://github.com/matdibu/concordance/actions/workflows/build.yml/badge.svg?branch=master&event=push)](https://github.com/matdibu/concordance/actions/workflows/build.yml)

Create an alphabetical list of all word occurrences, labeled with word frequencies.

Build prerequisites
-------------------

- Clang or GCC with c++20 support
- CMake version 3.20+

Build from source
-----------------

    mkdir build
    cd build
    cmake ..
    make

Run tests
---------

First, build it, then:

    make test


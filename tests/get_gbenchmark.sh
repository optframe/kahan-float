#!/bin/bash

# refresh submodules
git submodule update --init --recursive
git submodule update --recursive
# build thirdparty deps
if [ ! -d "$FOLDER" ] ; then
    (cd thirdparty && git clone https://github.com/google/googletest.git gbenchmark/googletest)
fi
(cd thirdparty && cd gbenchmark && mkdir build && cd build && cmake .. && make)

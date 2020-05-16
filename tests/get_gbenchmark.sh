#!/bin/bash

#CMAKE=/usr/local/cmake-3.17.2-Linux-x86_64/bin/cmake
CMAKE=cmake
# refresh submodules
git submodule update --init --recursive
git submodule update --recursive
# build thirdparty deps
#if [ ! -d "thirdparty/gbenchmark/googletest" ] ; then
#    (cd thirdparty && git clone https://github.com/google/googletest.git gbenchmark/googletest)
#fi
#export CXX=/usr/local/bin/g++
#(cd thirdparty && cd gbenchmark && rm -rf build && mkdir build && cd build && cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true .. && make )
# LTO not working on any gcc: -DBENCHMARK_ENABLE_LTO=true
(cd thirdparty && cd gbenchmark && rm -rf build && mkdir build && cd build && $CMAKE -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON -DCMAKE_BUILD_TYPE=Release .. && make )

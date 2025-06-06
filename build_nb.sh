#!/bin/sh -x

mkdir nobuild

PYTHON_INCLUDE=`pkg-config --cflags python3`

g++ nobnb.cpp -std=c++20 -fvisibility=hidden -DNDEBUG -DNB_COMPACT_ASSERTIONS $PYTHON_INCLUDE -fPIC -Iext/nanobind/include -Iext/nanobind/ext/robin_map/include -Os -c -o nobuild/nobnb.o

g++ ext/nanobind/src/nb_combined.cpp -std=c++20 -fvisibility=hidden -DNDEBUG -DNB_COMPACT_ASSERTIONS $PYTHON_INCLUDE -fPIC -Iext/nanobind/include -Iext/nanobind/ext/robin_map/include -O3 -fno-strict-aliasing -ffunction-sections -fdata-sections -c -o nobuild/libnanobind.o

g++ -shared -Wl,-s -Wl,--gc-sections nobuild/nobnb.o nobuild/libnanobind.o -o nobuild/nob.cpython-312-x86_64-linux-gnu.so

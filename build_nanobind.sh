#!/bin/sh -x

mkdir nobuild

g++ nobnb.cpp -std=c++17 -fvisibility=hidden -DNDEBUG -DNB_COMPACT_ASSERTIONS -I/usr/include/python3.12 -fPIC -Iext/nanobind/include -Iext/nanobind/ext/robin_map/include -Os -c -o nobuild/nobnb.o

g++ ext/nanobind/src/nb_combined.cpp -std=c++17 -fvisibility=hidden -DNDEBUG -DNB_COMPACT_ASSERTIONS -I/usr/include/python3.12 -fPIC -Iext/nanobind/include -Iext/nanobind/ext/robin_map/include -O3 -fno-strict-aliasing -ffunction-sections -fdata-sections -c -o nobuild/libnanobind.o

g++ -shared -Wl,-s -Wl,--gc-sections nobuild/nobnb.o nobuild/libnanobind.o -o nobuild/nob.cpython-312-x86_64-linux-gnu.so

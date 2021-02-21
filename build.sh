#!/bin/sh

mkdir -p build
pushd build
cmake ..
cmake --build . -j${nproc}

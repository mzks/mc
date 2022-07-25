#! /bin/bash

#rm -rf build

cmake -S . -B build
cmake --build build -j
cmake --install build --prefix .

cd build
ctest --output-on-failure

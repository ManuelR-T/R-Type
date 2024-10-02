#!/bin/bash
mkdir -p build-mingw32
cmake -DCMAKE_TOOLCHAIN_FILE=./cmake/mingw32-toolchain.cmake -S . -B build-mingw32
cmake --build build-mingw32 --config Release
#!/bin/bash

export PATH=~/soft/qemu-7.1.0/bin:$PATH
cd linux-aarch64-asan
# cp ../../assets/test.ppm ./
# cp ../../assets/test.pgm ./
ASAN_OPTIONS=detect_leaks=1 TESTS_EXECUTABLE_LOADER=qemu-aarch64 TESTS_EXECUTABLE_LOADER_ARGUMENTS="-L;/usr/aarch64-linux-gnu" ctest --output-on-failure -j 2
cd ..
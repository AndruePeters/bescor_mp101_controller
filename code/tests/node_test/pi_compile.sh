#!/bin/bash
export RPI_TC_PATH=~/Documents/pi/pi_toolchain
cmake . -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake -B_build

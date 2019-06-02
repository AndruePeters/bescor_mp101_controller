#!/bin/bash
export RPI_TC_PATH=~/Development/pi/pi_toolchain
rm -r _build
cmake . -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake -B_build
cd _build
make
scp main pi@192.168.1.162:~/Desktop
cd ..
ssh  -tt pi@192.168.1.162 << 'ENDSSH'
  echo "Trying to run main"
  ~/Desktop/main
  echo "Finished running main"
ENDSSH

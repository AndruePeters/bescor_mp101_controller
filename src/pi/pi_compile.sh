#!/bin/bash
export RPI_TC_PATH=~/Development/pi/pi_toolchain
rm -r _build
cmake . -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake -B_build
cd _build
make
scp main pi@192.168.1.162:~
cd .. 
scp config.yaml pi@192.168.1.162:~
echo Done
ssh  -tt -T pi@192.168.1.162 << 'ENDSSH'
  ./main
  echo "Finished running main"
ENDSSH

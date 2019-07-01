#!/bin/bash

# enable SPI
sudo echo "dtparam=spi=on" >> /boot/config.txt
sudo apt-get install git cmake build-essential -y

instFolder="camera_base_control"
mkdir $instFolder
cd $instFolder

# install rf24
git clone https://github.com/nRF24/RF24
cd RF24
./configure --driver=wiringPi
sudo make install
cd ..

# intall libyaml-cpp
git clone https://github.com/jbeder/yaml-cpp
cd yaml-cpp
cmake . -DBUILD_SHARED_LIBS=OFF -Bbuild
cd build
make
sudo make install
cd ../../

# install libsfml-dev
sudo apt-get install libsfml-dev -y

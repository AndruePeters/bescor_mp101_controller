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
sudo apt install libyaml-cpp-dev

# install libsfml-dev
sudo apt-get install libsfml-dev -y

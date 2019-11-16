#!/bin/bash

# enable SPI
sudo echo "dtparam=spi=on" >> /boot/config.txt
sudo apt-get install git cmake build-essential -y

instFolder="camera_base_control"
sudo -u mkdir $instFolder
sudo -u cd $instFolder

# install rf24
sudo -u git clone https://github.com/nRF24/RF24
cd RF24
./configure --driver=wiringPi
sudo make install
cd ..

# now clean up the RF24 build directory
cd ..
sudo rm -rf $instFolder

# intall libyaml-cpp
sudo apt install libyaml-cpp-dev -y

# install libsfml-dev
sudo apt-get install libsfml-dev -y

#install ncurses dev library
sudo apt-get install libncurses-dev -y


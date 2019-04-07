# Bescor MP101 Controller

Solution to wirelessly control the Bescor MP101 pan and tilt camera motor.
User uses a PS4 controller to control the motor.


## Software Used
* [wiringPi](http://wiringpi.com/)
* [RF24](https://github.com/nRF24/RF24)
* [yaml-cpp](https://github.com/jbeder/yaml-cpp)
* [sfml](https://www.sfml-dev.org/)

## Devices Used
* [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3/) (although, I am using a cheap alternative from China.)
* [Raspberry Pi 3 Model B](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)
* [nRF24L01+ 2.4GHz](https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf)
* [PS4 DualShock 4 Controller](https://www.playstation.com/en-us/explore/accessories/gaming-controllers/dualshock-4/)

# Installation

## Raspberry Pi
* **wiringPi**
  * wiringPi is installed by default. Follow [the instructions](http://wiringpi.com/download-and-install/) if you need to install it.
* **RF24**
  * `git clone https://github.com/nRF24/RF24`
  * `cd RF24/`
  * `./configure`
  * `sudo make install`
* **yaml-cpp**
  * Follow the instructions [here](https://github.com/jbeder/yaml-cpp).
* **sfml**
  * If you have Debian then run `sudo apt-get install libsfml-dev`
  * Otherwise, follow the [official guide](https://www.sfml-dev.org/tutorials/2.5/start-linux.php) to manually install it. Version is 2.5 at the time of writing.

## Cross-compile
I created a basic [toolchain](https://github.com/AndruePeters/pi_toolchain) to cross compile for the Raspberry Pi.
The toolchain is simply the standard Raspberry Pi toolchain with precompiled libraries and headers I commonly use for my projects.
I'll write a better cross-compile description once I get the full thing back up.

# Controls
* L1 - cycles camera selection left
* R1 - cycles camera selection right
* L  - Controls the camera.
* R
  * Moving up and down zooms in and out
  * Moving left and right focuses in and out


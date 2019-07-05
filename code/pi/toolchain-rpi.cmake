SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# set compiler paths using environmental variables
#SET(CMAKE_C_COMPILER $ENV{RPI_TC_PATH}/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER $ENV{RPI_TC_PATH}/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)


# set toolchain path using environmental variables
#SET(CMAKE_FIND_ROOT_PATH $ENV{RPI_TC_PATH}/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot)

# User our definition for compiler tools
#SET (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search for libraries and headers in the target directories only
# vSET (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_compile_options( -Wall -Wextra -Wpedantic -pedantic-errors)

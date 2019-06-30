cmake_minimum_required (VERSION 3.0)

# Name of the project
project (main)


set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs/)

# Add the local include directory and custom include directory
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}  ../shared/node/ ../shared/packet/ PUBLIC ${CUSTOM_LIB_PATH}/include)
message ("Include directories: ${CMAKE_CURRENT_SOURCE_DIR}   ${CUSTOM_LIB_PATH}/include")

link_directories ( ${CUSTOM_LIB_PATH}/bin ../../shared/node/ ../../shared/packet/ joystick/)

FILE (GLOB SRC_FILES *.cpp ../shared/node/*.cpp ../shared/packet/*.cpp joystick/*.cpp)
message (${SRC_FILES})
add_executable(main ${SRC_FILES})

# link pre-compiled libraries
# use rf24 for nrf2401+ and wiringPi for wiringPi
target_link_libraries(main   curses pthread wiringPi rf24 sfml-window sfml-system yaml-cpp)

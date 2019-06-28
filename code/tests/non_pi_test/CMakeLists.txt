cmake_minimum_required (VERSION 3.0)

# Name of the project
project (ps4_test)


# set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs/)
set(CMAKE_CXX_FLAGS "-Wall -g")

# Add the local include directory and custom include directory
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}  PUBLIC ../../pi/ ../../shared/packet/)
message ("Include directories: ${CMAKE_CURRENT_SOURCE_DIR} ")

# link_directories ( ${CUSTOM_LIB_PATH}/bin)

FILE (GLOB SRC_FILES *.cpp ../../pi/joystick/js.cpp ../../shared/packet/packet.cpp)
message (${SRC_FILES})
add_executable(ps4_test ${SRC_FILES})

# link pre-compiled libraries
target_link_libraries(ps4_test ncurses sfml-window sfml-system)

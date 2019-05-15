cmake_minimum_required (VERSION 3.0)

# Name of the project
project (node_test)


set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs/bin)

# Add the local include directory and custom include directory
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}  ../../shared/node/ PUBLIC ${CUSTOM_LIB_PATH}/include)
message ("Include directories: ${CMAKE_CURRENT_SOURCE_DIR}   ${CUSTOM_LIB_PATH}/include")

FILE (GLOB SRC_FILES *.cpp)
message (${SRC_FILES})
add_executable(test ${SRC_FILES})

# link pre-compiled libraries
# use rf24 for nrf2401+ and wiringPi for wiringPi
target_link_libraries(test rf24)

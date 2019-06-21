cmake_minimum_required (VERSION 3.0)

# Name of the project
project (ps4_test)


# set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs/)

# Add the local include directory and custom include directory
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}  ../../shared/node/ PUBLIC ${CUSTOM_LIB_PATH}/include)
message ("Include directories: ${CMAKE_CURRENT_SOURCE_DIR}   ${CUSTOM_LIB_PATH}/include")

# link_directories ( ${CUSTOM_LIB_PATH}/bin)

FILE (GLOB SRC_FILES *.cpp)
message (${SRC_FILES})
add_executable(ps4_test ${SRC_FILES})

# link pre-compiled libraries
target_link_libraries(ps4_test sfml-window sfml-system)

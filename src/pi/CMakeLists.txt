
set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs)

add_executable(camera_control main.cpp)
target_include_directories(camera_control PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(camera_control PRIVATE
    jsLib nodeLib packetLib
    curses pthread wiringPi rf24
    sfml-window sfml-system yaml-cpp
    CONAN_PKG::fmt CONAN_PKG::spdlog)


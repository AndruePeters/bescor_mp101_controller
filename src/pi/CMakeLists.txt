
set (CUSTOM_LIB_PATH $ENV{RPI_TC_PATH}/libs)

add_executable(camera_control main.cpp joystick/js.cpp)
target_include_directories(camera_control PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(camera_control PRIVATE nodeLib packetLib curses pthread wiringPi  rf24 sfml-window sfml-system yaml-cpp)


# link pre-compiled libraries
# use rf24 for nrf2401+ and wiringPi for wiringPi
#target_link_libraries(main    curses pthread wiringPi  rf24 sfml-window sfml-system yaml-cpp)

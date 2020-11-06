
add_executable(joystick_test joystick_test.cpp)

# link pre-compiled libraries
target_link_libraries(joystick_test PRIVATE jsLib ncurses sfml-window sfml-system)

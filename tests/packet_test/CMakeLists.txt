
add_executable(packet_test packet_test.cpp)

# link pre-compiled libraries
target_link_libraries(packet_test PRIVATE packetLib)

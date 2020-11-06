add_library(packetLib STATIC packet.cpp)
target_include_directories(packetLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
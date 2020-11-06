add_library(nodeLib STATIC node.cpp)
target_include_directories(nodeLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
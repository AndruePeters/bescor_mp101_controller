add_library(jsLib STATIC js.cpp)
target_include_directories(jsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
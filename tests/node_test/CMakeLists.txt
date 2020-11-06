add_executable(node_test node_test.cpp)

# link pre-compiled libraries
target_link_libraries(node_test PRIVATE nodeLib)

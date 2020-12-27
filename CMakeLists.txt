CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(ccnet1 C CXX)

set(PROJECT_OUT ccnet1)
set(CXX_FLAGS 
        -g
        -std=c++11)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_subdirectory(net)
add_subdirectory(example)
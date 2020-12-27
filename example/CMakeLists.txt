project(server C CXX)

set(PROJECT_OUT server)
set(CXX_FLAGS 
        -g
        -std=c++11)
set(CMAKE_CXX_STANDARD 11)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
include_directories(${PROJECT_SOURCE_DIR}/
                    ${PROJECT_SOURCE_DIR}/../net)
link_directories(${PROJECT_SOURCE_DIR}/../lib)

set(PROJECT_LIBRARIES ccnet)
set(PROJECT_SOURCES 
        server.cc
        EchoServer.cc)

add_executable(${PROJECT_OUT} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_OUT} ${PROJECT_LIBRARIES})
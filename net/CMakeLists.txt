
project(ccnet C CXX)

set(PROJECT_OUT ccnet)
set(CXX_FLAGS 
        -g
        -std=c++11
        -Wreorder
        )
set(CMAKE_CXX_STANDARD 11)


#开启gdb调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
include_directories(${PROJECT_SOURCE_DIR}/)
link_directories(${PROJECT_SOURCE_DIR}/../lib)

set(PROJECT_SOURCES 
    Acceptor.cc
    Buffer.cc
    Channel.cc
    EPollPoller.cc
    EventLoop.cc
    Socket.cc
    TcpAddr.cc
    TcpConnection.cc
    TcpServer.cc
    EventLoopThread.cc
    EventLoopThreadPool.cc)

set(PROJECT_LIBRARIES 
    pthread
    )

add_library(${PROJECT_OUT} SHARED ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_OUT} ${PROJECT_LIBRARIES})
# add_library(${PROJECT_OUT}"_static" STATIC ${PROJECT_SOURCES})
# target_link_libraries(${PROJECT_OUT}"_static" ${PROJECT_LIBRARIES})
# #指定静态库的输出名称
# set_target_properties(${PROJECT_OUT}"_static" PROPERTIES OUTPUT_NAME ${PROJECT_OUT})
# #使动态库和静态库同时存在
# set_target_properties(${PROJECT_OUT} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# set_target_properties(${PROJECT_OUT}"_static" PROPERTIES CLEAN_DIRECT_OUTPUT 1)


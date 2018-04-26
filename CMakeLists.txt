cmake_minimum_required(VERSION 2.8)
project(dstarlite)
SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

add_executable(dstarlite src/main.cpp src/heap.cpp src/maze.cpp src/dstarlite.cpp)
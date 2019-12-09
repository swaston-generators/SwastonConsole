cmake_minimum_required(VERSION 3.0)
project(SwastonConsole)

set(CMAKE_CXX_STANDARD 98)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZERE} -O3 -Os")
endif ()

add_executable(SwastonConsole main.cpp)
target_include_directories(SwastonConsole PRIVATE include/)
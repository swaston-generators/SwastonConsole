cmake_minimum_required (VERSION 3.0)
project (SwastonConsole)

set(CMAKE_CXX_STANDARD 98)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE MinSizeRel)
endif()

file(GLOB SOURCES
        ./*.cpp
        )

add_executable(SwastonConsole ${SOURCES})
target_include_directories(SwastonConsole PRIVATE ./include/)
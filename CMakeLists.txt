cmake_minimum_required(VERSION 2.8)
project(cish CXX)

option(test "Build all tests" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE CISH_SRCS src/**.cpp)
file(GLOB_RECURSE CISH_HDRS src/**.h)

add_library(cish ${CISH_SRCS} ${CISH_HDRS})
set_property(TARGET cish PROPERTY CXX_STANDARD 17)
set_property(TARGET cish PROPERTY CXX_STANDARD_REQUIRED ON)

SET(CISH_LIBRARY "cish")
SET(CISH_HEADER_PATH "${CMAKE_SOURCE_DIR}/src/")


if (test)
    add_subdirectory(test)
endif()

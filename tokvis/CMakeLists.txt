cmake_minimum_required(VERSION 3.5)
project(cish_tokvis CXX)

file(GLOB_RECURSE TOKVIS_SRCS src/**.cpp)
file(GLOB_RECURSE TOKVIS_HDRS src/**.h)

add_executable(tokvis ${TOKVIS_SRCS} ${TOKVIS_HDRS})
add_dependencies(tokvis ${CISH_LIBRARY})
set_property(TARGET tokvis PROPERTY CXX_STANDARD 23)
set_property(TARGET tokvis PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(tokvis ${CISH_LIBRARY})
include_directories(${CISH_HEADER_PATH})
include_directories(SYSTEM ${CISH_HEADER_PATH})


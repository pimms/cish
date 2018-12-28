cmake_minimum_required(VERSION 2.8)
project(cish_cli CXX)

file(GLOB_RECURSE CLI_SRCS src/**.cpp)
file(GLOB_RECURSE CLI_HDRS src/**.h)

add_executable(cish_cli ${CLI_SRCS} ${CLI_HDRS})
add_dependencies(cish_cli ${CISH_LIBRARY})
set_property(TARGET cish_cli PROPERTY CXX_STANDARD 17)
set_property(TARGET cish_cli PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(cish_cli ${CISH_LIBRARY})
include_directories(${CISH_HEADER_PATH})
include_directories(SYSTEM ${CISH_HEADER_PATH})

install(TARGETS cish_cli DESTINATION bin)

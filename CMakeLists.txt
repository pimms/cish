cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(cish CXX)

option(build_cish_test "Build all tests" ON)
option(build_cish_cli "Build the cish CLI tool" ON)
option(build_tokvis "Build the token visualizer" ON)
option(use_antlr "Use antlr" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG ${CMAKE_CXX_FLAGS_DEBUG} -g")

# -- Run ANTLR4 Generation -- #

if (use_antlr)
    execute_process(
        RESULT_VARIABLE antlr_result
        OUTPUT_VARIABLE antlr_stdout
        ERROR_VARIABLE antlr_stderr
        COMMAND antlr "${CMAKE_CURRENT_SOURCE_DIR}/grammar/CM.g4" -o ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/antlr -visitor -Dlanguage=Cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT antlr_result STREQUAL "0")
        message(FATAL_ERROR "antlr generation failed: ${antlr_stdout}\n${antlr_stderr}")
    endif()

    file(GLOB_RECURSE CISH_SRCS src/**.cpp)
    file(GLOB_RECURSE CISH_HDRS src/**.h)

    # -- Find ANTLR4 runtime -- #
    find_library(ANTLR_LIBRARY
        HINTS "/opt/homebrew/lib" "/usr/local/lib"
        NAMES "libantlr4-runtime.a" "antlr4-runtime")
    if ("${ANTLR_LIBRARY}" STREQUAL "ANTLR_LIBRARY-NOTFOUND")
        message(SEND_ERROR "Failed to find antlr4-runtime library")
    endif()

    # The generated antlr-sources include "antlr4-runtime.h", so we need to find
    # the parent directory and make sure that the root header actually exists.
    find_path(ANTLR_INCLUDE_ROOT_DIR
        HINTS "/opt/homebrew/include"
        NAMES "antlr4-runtime")
    if ("${ANTLR_INCLUDE_ROOT_DIR}" STREQUAL "ANTLR_INCLUDE_ROOT_DIR-NOTFOUND")
        message(SEND_ERROR "Failed to find antlr4-runtime header directory")
    endif()

    set(ANTLR_INCLUDE_DIR "${ANTLR_INCLUDE_ROOT_DIR}/antlr4-runtime")
    if (NOT EXISTS "${ANTLR_INCLUDE_DIR}/antlr4-runtime.h")
        message(SEND_ERROR "Failed to locate 'antlr4-runtime.h' (${ANTLR_INCLUDE_ROOT_DIR}, ${ANTLR_INCLUDE_DIR})")
    endif()
else()
    file(GLOB_RECURSE CISH_SRCS src/Exception.cpp src/parse/*.cpp src/lex/*.cpp)
    file(GLOB_RECURSE CISH_HDRS src/Exception.h src/parse/*.h src/lex/*.h)
endif()

add_library(cish ${CISH_SRCS} ${CISH_HDRS})
set_property(TARGET cish PROPERTY CXX_STANDARD 23)
set_property(TARGET cish PROPERTY CXX_STANDARD_REQUIRED ON)
set(CISH_LIBRARY "cish")
set(CISH_HEADER_PATH "${CMAKE_SOURCE_DIR}/src/")

if (use_antlr)
    target_link_libraries(cish ${ANTLR_LIBRARY})
    include_directories(${ANTLR_INCLUDE_DIR})

    if (build_cish_cli)
        add_subdirectory(cli)
    endif()
    if (build_cish_test)
        add_subdirectory(test)
    endif()
endif()

if (build_tokvis)
    add_subdirectory(tokvis)
endif()

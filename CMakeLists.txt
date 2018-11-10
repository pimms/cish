cmake_minimum_required(VERSION 2.8)
project(cish CXX)

option(test "Build all tests" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")


# -- Run ANTLR4 Generation -- #
execute_process(
    RESULT_VARIABLE antlr_result
	OUTPUT_VARIABLE antlr_stdout
	ERROR_VARIABLE antlr_stderr
    COMMAND antlr4 "${CMAKE_CURRENT_SOURCE_DIR}/grammar/CM.g4" -o src/ast/antlr -visitor -Dlanguage=Cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
if(NOT antlr_result STREQUAL "0")
	message(FATAL_ERROR "antlr generation failed: ${antlr_stdout}\n${antlr_stderr}")
endif()



file(GLOB_RECURSE CISH_SRCS src/**.cpp)
file(GLOB_RECURSE CISH_HDRS src/**.h)

add_library(cish ${CISH_SRCS} ${CISH_HDRS})
set_property(TARGET cish PROPERTY CXX_STANDARD 17)
set_property(TARGET cish PROPERTY CXX_STANDARD_REQUIRED ON)

set(CISH_LIBRARY "cish")
set(CISH_HEADER_PATH "${CMAKE_SOURCE_DIR}/src/")

foreach(source IN LISTS CISH_SRCS)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" source_path "${source_path}")
    string(REPLACE "/" "\\" source_path_xcode "${source_path}")
    source_group("${source_path_xcode}" FILES "${source}")
endforeach()
foreach(source IN LISTS CISH_HDRS)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" source_path "${source_path}")
    string(REPLACE "/" "\\" source_path_xcode "${source_path}")
    source_group("${source_path_xcode}" FILES "${source}")
endforeach()




# -- Find ANTLR4 runtime -- #
find_library(ANTLR_LIBRARY NAMES "antlr4-runtime")
if ("${ANTLR_LIBRARY}" STREQUAL "ANTLR_LIBRARY-NOTFOUND")
    message(SEND_ERROR "Failed to find antlr4-runtime library")
endif()

# The generated antlr-sources include "antlr4-runtime.h", so we need to find
# the parent directory and make sure that the root header actually exists.
find_path(ANTLR_INCLUDE_ROOT_DIR NAMES "antlr4-runtime")
if ("${ANTLR_INCLUDE_ROOT_DIR}" STREQUAL "ANTLR_INCLUDE_ROOT_DIR-NOTFOUND")
    message(SEND_ERROR "Failed to find antlr4-runtime header directory")
endif()

set(ANTLR_INCLUDE_DIR "${ANTLR_INCLUDE_ROOT_DIR}/antlr4-runtime")
if (NOT EXISTS "${ANTLR_INCLUDE_DIR}/antlr4-runtime.h")
    message(SEND_ERROR "Failed to locate 'antlr4-runtime.h' (${ANTLR_INCLUDE_ROOT_DIR}, ${ANTLR_INCLUDE_DIR})")
endif()

target_link_libraries(cish ${ANTLR_LIBRARY})
include_directories(${ANTLR_INCLUDE_DIR})



# -- Add tests -- #
if (test)
    add_subdirectory(test)
endif()

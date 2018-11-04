cmake_minimum_required(VERSION 2.8)
project(cish_test CXX)

enable_testing()


# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

if ("${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set(LIBGTESTA "${binary_dir}/lib/Debug/libgtestd.a")
    set(LIBGTESTMAINA "${binary_dir}/lib/Debug/libgtest_maind.a")
else()
    set(LIBGTESTA "${binary_dir}/lib/libgtest.a")
    set(LIBGTESTMAINA "${binary_dir}/lib/libgtest_main.a")
endif()

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${LIBGTESTA}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_library(libgtest_main IMPORTED STATIC GLOBAL)
add_dependencies(libgtest_main gtest_main)
set_target_properties(libgtest_main PROPERTIES
    "IMPORTED_LOCATION" "${LIBGTESTMAINA}"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)



# Setup the actual test suite
file(GLOB_RECURSE TEST_SRCS src/**.cpp)
file(GLOB_RECURSE TEST_HDRS src/**.h)
add_executable(cishtest ${TEST_SRCS} ${TEST_HDRS})
add_dependencies(cishtest ${CISH_LIBRARY})
set_property(TARGET cishtest PROPERTY CXX_STANDARD 17)
set_property(TARGET cishtest PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(cishtest ${CISH_LIBRARY} libgtest libgtest_main)
include_directories(${CISH_HEADER_PATH})
include_directories(SYSTEM ${CISH_HEADER_PATH} "${source_dir}/googletest/include")

add_test(unit_tests "cishtest")
add_custom_target(check COMMAND cishtest)

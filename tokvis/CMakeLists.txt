file(GLOB_RECURSE TOKVIS_SRCS src/**.cpp)
file(GLOB_RECURSE TOKVIS_HDRS src/**.h)

add_executable(tokvis ${TOKVIS_SRCS} ${TOKVIS_HDRS})
add_dependencies(tokvis ${CISH_LIBRARY})
set_property(TARGET tokvis PROPERTY CXX_STANDARD 23)
set_property(TARGET tokvis PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(imgui CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(tokvis PRIVATE ${CISH_LIBRARY} imgui::imgui SDL3::SDL3 OpenGL::GL)

include_directories(${CISH_HEADER_PATH})
include_directories(SYSTEM ${CISH_HEADER_PATH})


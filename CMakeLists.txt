cmake_minimum_required(VERSION 3.10)

project(learn_opengl)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
set(OPENGL_LIB X11 dl pthread Xxf86vm Xrandr Xcursor Xinerama GL)
endif()

link_libraries(${OPENGL_LIB})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# 3rdparty headers
include_directories(3rdparty/glad/include 3rdparty/glfw3/include)

add_subdirectory(3rdparty)

link_libraries(glad glfw)
add_subdirectory(hello_opengl)
add_subdirectory(hello_opengl_with_glew)
add_subdirectory(hello_triangle)
add_subdirectory(hello_rectangle)

# copy compile_commands.json
configure_file(${CMAKE_BINARY_DIR}/compile_commands.json ./compile_commands.json)

cmake_minimum_required(VERSION 3.10)

project(learn_opengl)

set(OPENGL_LIB glfw GLEW X11 dl pthread Xxf86vm Xrandr Xcursor Xinerama GL)

link_libraries(${OPENGL_LIB})

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(hello_opengl)
add_subdirectory(hello_opengl_with_glew)
add_subdirectory(hello_triangle)
add_subdirectory(hello_rectangle)

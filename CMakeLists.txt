cmake_minimum_required(VERSION 3.10)

project(learn_opengl)

set(OPENGL_LIB glfw GLEW X11 dl pthread Xxf86vm Xrandr Xcursor Xinerama GL)

add_subdirectory(hello_opengl)
add_subdirectory(hello_opengl_with_glew)
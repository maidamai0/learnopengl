cmake_minimum_required(VERSION 3.10)

project(learn_opengl)

set(OPENGL_LIB glfw3 X11 dl pthread Xxf86vm Xrandr Xcursor Xinerama GL)

add_subdirectory(hello_opengl)
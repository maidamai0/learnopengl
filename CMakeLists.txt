cmake_minimum_required(VERSION 3.10)

project(learn_opengl)

set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
set(OPENGL_LIB X11 dl pthread Xxf86vm Xrandr Xcursor Xinerama GL stdc++fs)
endif()

link_libraries(${OPENGL_LIB})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# generate compile_commands.json if possible
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

# library compilation
set(BUILD_SHARED_LIBS OFF)

# 3rdparty headers
include_directories(./ 
                    3rdparty/glad/include
                    3rdparty/glfw3/include
                    3rdparty/fmt/include
                    3rdparty/linmath/include
                    3rdparty/stb/include
                    3rdparty/cxxopts/include)

# compile 3rdparty libs
add_subdirectory(3rdparty)

# glfw required this macro for DLL version
# add_compile_definitions(GLFW_DLL)
add_compile_options(-DGLFW_DLL)

# std_image implementation switch
# add_compile_definitions(STB_IMAGE_IMPLEMENTATION)
add_compile_options(-DSTB_IMAGE_IMPLEMENTATION)

# add 3rdparty lib
link_libraries(fmt glfw glad)

# compile common libs
add_subdirectory(common)

# compile executables
add_subdirectory(hello_opengl)
add_subdirectory(vao_and_shader)
add_subdirectory(hello_triangle)
add_subdirectory(hello_rectangle)
add_subdirectory(get_started_with_glfw)
add_subdirectory(shaders)
add_subdirectory(textures)
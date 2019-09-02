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

# add 3rdparty lib
link_libraries(fmt glfw glad)

# glfw required this macro for DLL version
add_compile_definitions(GLFW_DLL)

# std_image implementation switch
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

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
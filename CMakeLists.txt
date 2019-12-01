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

# clang-tidy
# TODO header files-not found
# set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,cppcoreguidelines-*,clang-analyzer-*,modernize-*,google-* -fix --list-checks)

# library compilation
set(BUILD_SHARED_LIBS OFF)

# 3rdparty headers
include_directories(./ 
                    3rdparty/glad/include
                    3rdparty/glfw3/include
                    3rdparty/fmt/include
                    3rdparty/linmath/include
                    3rdparty/stb/include
                    3rdparty/cxxopts/include
                    3rdparty/glm/include)

# compile 3rdparty libs
add_subdirectory(3rdparty)

# glfw required this macro for DLL version
# add_compile_definitions(GLFW_DLL)
add_compile_options(-DGLFW_DLL)

# std_image implementation switch
# add_compile_definitions(STB_IMAGE_IMPLEMENTATION)
add_compile_options(-DSTB_IMAGE_IMPLEMENTATION)

# compile flags
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(PEDANTIC_COMPILE_FLAGS -pedantic-errors -Wall -Wextra -pedantic
      -Wold-style-cast -Wundef
      -Wredundant-decls -Wwrite-strings -Wpointer-arith
      -Wcast-qual -Wformat=2 -Wmissing-include-dirs
      -Wcast-align -Wnon-virtual-dtor
      -Wctor-dtor-privacy -Wdisabled-optimization
      -Winvalid-pch -Woverloaded-virtual
      -Wconversion
      -Wno-ctor-dtor-privacy -Wno-format-nonliteral -Wno-shadow)
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wnoexcept
         -Wno-dangling-else -Wno-unused-local-typedefs)
  endif ()
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wdouble-promotion
          -Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast
          -Wvector-operation-performance -Wsized-deallocation)
  endif ()
  if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wshift-overflow=2
          -Wnull-dereference -Wduplicated-cond)
  endif ()
  set(WERROR_FLAG -Werror)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(PEDANTIC_COMPILE_FLAGS -Wall -Wextra -pedantic -Wconversion
      -Wno-sign-conversion)
  check_cxx_compiler_flag(-Wzero-as-null-pointer-constant HAS_NULLPTR_WARNING)
  if (HAS_NULLPTR_WARNING)
    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
        -Wzero-as-null-pointer-constant)
  endif ()
  set(WERROR_FLAG -Werror)
endif ()

if (MSVC)
  set(COMMON_COMPILE_FLAGS /EHsc)
  set(PEDANTIC_COMPILE_FLAGS /W4)
  set(WERROR_FLAG /WX)
endif ()

add_compile_options(${COMMON_COMPILE_FLAGS} ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})

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
add_subdirectory(transformations)
add_subdirectory(coordinates)
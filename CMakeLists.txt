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
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

# library compilation
set(BUILD_SHARED_LIBS ON)

# for msvc max min confilct
add_compile_definitions(NOMINMAX)

# dependency headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/dependency/glad/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/dependency/stb
)

add_subdirectory(dependency)

# clang-tidy
# TODO header files-not found
# set(CMAKE_CXX_CLANG_TIDY clang-tidy --fix --fix-errors)

# std_image implementation switch
# add_compile_definitions(STB_IMAGE_IMPLEMENTATION)
add_compile_options(-DSTB_IMAGE_IMPLEMENTATION)

# resource path
add_definitions(-DRES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

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

  if (HAS_NULLPTR_WARNING)
    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
        -Wzero-as-null-pointer-constant)
  endif ()
  set(WERROR_FLAG -Werror)
endif ()

if (MSVC)
  set(COMMON_COMPILE_FLAGS /EHsc)
  set(PEDANTIC_COMPILE_FLAGS /W3 /wd4267)
  set(WERROR_FLAG /WX)
  set(CMAKE_WIN32_EXECUTABLE TRUE)
endif ()

add_compile_options(${COMMON_COMPILE_FLAGS} ${PEDANTIC_COMPILE_FLAGS} ${WERROR_FLAG})

# add dependency lib
link_libraries(glfw glad assimp::assimp freetype fmt::fmt-header-only cxxopts::cxxopts glm)

# compile common libs
add_subdirectory(common)

execute_process(
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/glsl_2_header.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND_ECHO STDOUT
)

# compile executables
# get subdir
# https://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/CMakeLists.txt)
        if(subdir STREQUAL "common" OR subdir STREQUAL "dependency")
          continue()
        endif()

    message(STATUS "project ${subdir} added")
    add_subdirectory(${subdir})
    endif()
endforeach()

project(hello_opengl)

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(hello_opengl ${src})
target_include_directories(hello_opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hello_opengl ${OPENGL_LIB})
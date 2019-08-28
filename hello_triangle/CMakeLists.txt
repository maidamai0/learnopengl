file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")

# add excutable
add_executable(hello_triangle ${src})
target_link_libraries(hello_triangle common)

# copy shaders source code
configure_file(fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fs.glsl COPYONLY)
configure_file(vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vs.glsl COPYONLY)

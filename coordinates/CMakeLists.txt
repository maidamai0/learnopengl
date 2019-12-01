file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(coordinates ${src})

# copy shader source code
configure_file(coordinates_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/coordinates_fs.glsl COPYONLY)
configure_file(coordinates_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/coordinates_vs.glsl COPYONLY)
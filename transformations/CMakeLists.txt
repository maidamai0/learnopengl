file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(transformations ${src})

# copy shader source code
configure_file(transformations_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/transformations_fs.glsl COPYONLY)
configure_file(transformations_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/transformations_vs.glsl COPYONLY)
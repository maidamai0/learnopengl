file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(shaders ${src})# copy shaders source code
configure_file(shaders_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders_fs.glsl COPYONLY)
configure_file(shaders_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders_vs.glsl COPYONLY)
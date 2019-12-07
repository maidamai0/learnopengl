file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(color ${src})

# copy shader source code
configure_file(color_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/color_fs.glsl COPYONLY)
configure_file(color_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/color_vs.glsl COPYONLY)
configure_file(light_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/light_fs.glsl COPYONLY)
configure_file(light_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/light_vs.glsl COPYONLY)
file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(basic_lighting ${src})

# copy shader source code
configure_file(basic_lighting_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basic_lighting_fs.glsl COPYONLY)
configure_file(basic_lighting_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basic_lighting_vs.glsl COPYONLY)
configure_file(basic_lighting_light_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basic_lighting_light_fs.glsl COPYONLY)
configure_file(basic_lighting_light_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basic_lighting_light_vs.glsl COPYONLY)
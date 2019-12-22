file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(lighting_map ${src})

# copy shader source code
configure_file(lighting_map_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lighting_map_fs.glsl COPYONLY)
configure_file(lighting_map_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lighting_map_vs.glsl COPYONLY)
configure_file(lighting_map_light_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lighting_map_light_fs.glsl COPYONLY)
configure_file(lighting_map_light_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lighting_map_light_vs.glsl COPYONLY)
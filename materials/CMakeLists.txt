file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(materials ${src})

# copy shader source code
configure_file(materials_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/materials_fs.glsl COPYONLY)
configure_file(materials_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/materials_vs.glsl COPYONLY)
configure_file(materials_light_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/materials_light_fs.glsl COPYONLY)
configure_file(materials_light_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/materials_light_vs.glsl COPYONLY)
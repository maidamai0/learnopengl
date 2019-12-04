file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(camera ${src})

# copy shader source code
configure_file(camera_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/camera_fs.glsl COPYONLY)
configure_file(camera_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/camera_vs.glsl COPYONLY)
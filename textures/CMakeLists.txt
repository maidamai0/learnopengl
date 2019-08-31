file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(textures ${src})

# copy shader source code
configure_file(textures_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures_fs.glsl COPYONLY)
configure_file(textures_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/textures_vs.glsl COPYONLY)

# image
configure_file(winnie_the_pooh.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/winnie_the_pooh.jpg COPYONLY)
configure_file(xijinping.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xijinping.jpg COPYONLY)
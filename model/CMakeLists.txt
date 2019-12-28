file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(model ${src})

# copy shader source code
configure_file(model_fs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model_fs.glsl COPYONLY)
configure_file(model_vs.glsl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/model_vs.glsl COPYONLY)
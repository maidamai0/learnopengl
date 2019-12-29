file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(frame_buffer ${src})

# copy shader source code
configure_file(frame_buffer.fs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/frame_buffer.fs COPYONLY)
configure_file(frame_buffer.vs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/frame_buffer.vs COPYONLY)
configure_file(frame_buffer_screen.fs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/frame_buffer_screen.fs COPYONLY)
configure_file(frame_buffer_screen.vs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/frame_buffer_screen.vs COPYONLY)
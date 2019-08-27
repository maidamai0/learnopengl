project(get_started_with_glfw)

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(get_started_with_glfw ${src})
target_link_libraries(get_started_with_glfw common)
project(get_started_with_glfw)

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(get_started_with_glfw ${src})
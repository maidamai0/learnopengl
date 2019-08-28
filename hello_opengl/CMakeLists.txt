project(hello_opengl)

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(hello_opengl ${src})
target_link_libraries(hello_opengl common)
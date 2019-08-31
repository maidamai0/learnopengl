project(hello_opengl)

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(hello_opengl ${src})
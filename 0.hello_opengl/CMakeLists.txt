project(hello_opengl)
add_compile_definitions(APP_NAME="${PROJECT_NAME}")

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(hello_opengl ${src})
file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")

# add excutable
add_executable(hello_triangle WIN32 ${src})
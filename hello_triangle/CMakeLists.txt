file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")
add_compile_definitions(APP_NAME="HelloTriangle")
# add excutable
add_executable(hello_triangle WIN32 ${src})
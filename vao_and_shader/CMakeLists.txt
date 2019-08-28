file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")

# add excutable
add_executable(vao_and_shader ${src})
target_link_libraries(vao_and_shader common)
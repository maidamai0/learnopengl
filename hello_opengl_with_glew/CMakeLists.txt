file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")

# add excutable
add_executable(hello_opengl_with_glew ${src})

target_link_libraries(hello_opengl_with_glew ${OPENGL_LIB})
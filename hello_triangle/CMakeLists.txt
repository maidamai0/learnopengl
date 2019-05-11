file(GLOB src 
"*.cpp"
"*.h"
"*.glsl")

# copy fragment shader source code
set(IN_FS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/fs.glsl")
set(OUT_FS_FILE "${CMAKE_BINARY_DIR}/fs.glsl")
configure_file(${IN_FS_FILE} ${OUT_FS_FILE} COPYONLY)

# copy veterx shader source code
set(IN_VS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vs.glsl")
set(OUT_VS_FILE "${CMAKE_BINARY_DIR}/vs.glsl")
configure_file(${IN_VS_FILE} ${OUT_VS_FILE} COPYONLY)

# add excutable
add_executable(hello_triangle ${src})

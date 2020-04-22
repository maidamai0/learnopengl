get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# add root path macro to source
add_definitions(-DSHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

file(GLOB src 
"*.cpp"
"*.h")

# add excutable
add_executable(${TARGET} ${src})
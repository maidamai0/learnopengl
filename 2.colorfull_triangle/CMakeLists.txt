get_filename_component(_app ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_compile_definitions(APP_NAME="${_app}")

file(GLOB src *.cpp)

add_executable(${_app} ${src})
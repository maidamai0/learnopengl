file(GLOB src 
"*.cpp"
"*.h")

# export
add_compile_definitions(COMMON_DLL_EXPORT)

# add add library
add_library(common SHARED ${src})

file(GLOB test_src
    "*cxx")

# add test
add_executable(common_test ${test_src})
target_link_libraries(common_test common glfw)
add_test(common_test ${EXECUTABLE_OUTPUT_PATH}/common_test)
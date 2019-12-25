add_library(glad src/glad.c)
target_include_directories(glad PRIVATE include)
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT)
target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT_BUILD)

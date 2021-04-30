add_subdirectory(glad)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "disable assimp test" FORCE)
add_subdirectory(assimp)

add_subdirectory(fmt)
add_subdirectory(cxxopts)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(freetype)
set(SPDLOG_FMT_EXTERNAL_HO ON CACHE BOOL "use external fmt" FORCE)
add_subdirectory(spdlog)
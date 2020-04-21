/**
 * @file path.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief   get common used paths
 * @version 0.1
 * @date 2020-04-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SHADERS_PATH
#define SHADERS_PATH ""
#endif

#include <string>

/**
 * @brief Get the shader full path
 *
 * @warning define SHADERS_PATH in cmake before use this header, or you will propably get the wrong
 * path
 *
 * @param shader_name       shade file name
 * @return std::string      shade file full path
 */
auto get_shader_full_path(std::string shader_name) -> std::string {
    return std::string(SHADERS_PATH) + shader_name;
}
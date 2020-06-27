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

#ifndef RES_PATH
#define RES_PATH ""
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
inline auto get_shader_full_path(std::string shader_name) -> std::string {
    return std::string(SHADERS_PATH) + shader_name;
}

/**
 * @brief Get the resource full path object
 * 
 * @param resource_name     typically a image name with extension
 * @return std::string      full path of this resource file 
 */
inline auto get_resource_full_path(std::string&& resource_name) -> std::string {
    return std::string(RES_PATH) + resource_name;
}
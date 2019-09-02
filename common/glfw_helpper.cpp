#pragma once

/**
 * @file glfw_helpper.cpp
 * @author tonghao.yuan
 * @brief some hellper functions for glfw
 * @version 0.1
 * @date 2019-08-28
 * 
 */

#include "common/glfw_helpper.h"

#include <fstream>
#include <sstream>

void err_callback(int err, const char *msg) {
    fmt::print("error:{}[{}]\n", err, msg);
}

void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        fmt::print("Escape pressed\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE); // not work
    }
}

void resize_callback(GLFWwindow *pWd, int w, int h) {
    int h_old = 0;
    int w_old = 0;
    glfwGetFramebufferSize(pWd, &w_old, &h_old);
    fmt::print("resize from{}x{} to {}x{}\n", w_old, h_old, w, h);
    glViewport(0, 0, w, h); // what's the purpose?
}

std::string read_shader(std::string && shader_file_path) {
    std::ifstream shader_reader;
    std::stringstream buff;
    shader_reader.open(shader_file_path);
    if(!shader_reader.is_open()){
        throw::std::exception("file not exist");
    }
    buff << shader_reader.rdbuf();

    return buff.str();
}
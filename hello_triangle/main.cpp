/**
 * @file gl.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-04-16
 *
 * @copyright Copyright (c) 2019
 *
 */

// glew
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;

static const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

/**
 * @brief glfw error callback function
 *
 * @param err	error number
 * @param msg 	description
 */
void err_callback(int err, const char *msg) {
    cout << "err:" << err << msg << endl;
}

static void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cout << "Escape pressed" << endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void resize_callback(GLFWwindow *pWd, int w, int h) {
    int h_old = 0;
    int w_old = 0;
    glfwGetFramebufferSize(pWd, &w_old, &h_old);
    cout << "resize from" << w_old << ":" << h_old << " to " << w << ":" << h;
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow *pWd) {
    (void)pWd;
    // do nothing
}

bool check_compile(const GLuint shader, const GLchar *const shader_source) {
    GLint success = 0;
    GLchar msg[512] = {0};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg);
        std::cerr << "compile shader failed:" << msg << "\nsource:\n" << shader_source << "\n";
        return false;
    }

    return true;
}

bool check_link(const GLuint link) {
    GLint success = 0;
    GLchar msg[512] = {0};

    glGetProgramiv(link, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(link, sizeof(msg), nullptr, msg);
        std::cerr << "compile shader failed:" << msg << "\nsource:\n";
        return false;
    }

    return true;
}

string read_whole_file(const char *file) {
    ifstream input(file);

    if (!input.is_open()) {
        cout << "open " << file << " failed\n";
        return string();
    }

    stringstream sstr;
    string out;

    while (input >> sstr.rdbuf()) {
        // do nothing
    };

    out = sstr.str();

    return out;
}

/**
 * @brief main function
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // initialize glfw
    if (!glfwInit()) {
        cout << "init error, exit" << endl;
        return -1;
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        cout << "create window failed!" << endl;
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize glew
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cout << "glew initialize failed:" << glewGetErrorString(err) << endl;
        return -1;
    }

    // create shader
    const GLchar *vertex_shader_source = nullptr;
    const char *fragment_shader_source = nullptr;

    string tmp = read_whole_file("vs.glsl");
    vertex_shader_source = tmp.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    if (!check_compile(vertex_shader, vertex_shader_source)) {
        return -1;
    }

    tmp = read_whole_file("fs.glsl");
    fragment_shader_source = tmp.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    if (!check_compile(fragment_shader, fragment_shader_source)) {
        return -1;
    }

    // create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    if (!check_link(shader_program)) {
        return -1;
    }

    // delete shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // create vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell opengl how to interpret our data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // unbind vao
    glBindVertexArray(0);

    // set viewport
    int height = 0, width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // process input
        process_input(pWd);

        // clear color
        glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // event loop
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    cout << "user request to close this window!" << endl;

    // delete vertex array object and vertex buffer object
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

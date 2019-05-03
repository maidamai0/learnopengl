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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

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
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
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
    // set error callback
    glfwSetErrorCallback(err_callback);

    // initialize
    if (!glfwInit()) {
        cout << "init error, exit" << endl;
    }

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        cout << "create window failed!" << endl;
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // init glew
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cout << "glew initialize failed:" << glewGetErrorString(err) << endl;
        return -1;
    }

    // get gl info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("rederer is %s\n", renderer);
    printf("version is %s\n", version);

    // config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // triangle point
    float points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5, 0.0f};

    // copy data to graphical card with vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    // vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // shader
    const char *vertex_shader = nullptr;
    //        "#version 150\n"
    //        "in vec3 vp;"
    //        "void main() {"
    //        "  gl_Position = vec4(vp, 1.0);"
    //        "}";

    const char *fragment_shader = nullptr;
    //        "#version 150\n"
    //        "out vec4 frag_colour;"
    //        "void main() {"
    //        "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    //        "}";

    ifstream shader_reader;
    stringstream buff;
    shader_reader.open("vs.glsl");
    buff << shader_reader.rdbuf();
    vertex_shader = buff.str().c_str();

    //    shader_reader.clear();
    shader_reader.open("fs.glsl");
    buff << shader_reader.rdbuf();
    fragment_shader = buff.str().c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    // gpu shade program
    GLuint shade_proram = glCreateProgram();
    glAttachShader(shade_proram, vs);
    glAttachShader(shade_proram, fs);
    glLinkProgram(shade_proram);

    // clear color
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup environment
        glUseProgram(shade_proram);
        glBindVertexArray(vao);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // poll event
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    cout << "user request to close this window!" << endl;

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief
 * @version 0.1
 * @date 2019-08-28
 *
 */

#include <algorithm>
#include <array>

#include "common/color.h"
#include "common/glfw_helpper.h"
#include "common/log.h"
#include "common/win_main.h"
#include "fs.glsl.h"
#include "vs.glsl.h"

// clang-format off
// point for rectangle
static constexpr std::array<float, 3*4> position =
{
    0.5F, 0.5F, 0.0F,
    0.5F, -0.5F, 0.0F,
    -0.5F, -0.5F, 0.0F,
    -0.5F, 0.5F, 0.0F
};

// index for two triangles
static constexpr std::array<unsigned int, 3*2> indices =
{
    0,1,3,
    1,2,3
};
// clang-format on

auto main(int argc, char **argv) -> int {
  (void)argc;
  (void)argv;

  // clang-format off
    static constexpr std::array<float, position.size()> color =
    {
        1.0F, 1.0F, 0.0F,
        0.0F, 1.0F, 1.0F,
        1.0F, 0.0F, 1.0F,
        0.5F, 0.5F, 0.5F
    };
  // clang-format on

  // initialize glfw
  GLFW_GUARD;

  // set error callback
  glfwSetErrorCallback(err_callback);

  // create a window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  auto *pWd = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
  if (pWd == nullptr) {
    LOGE("create window failed!");
  }

  // set key callback
  glfwSetKeyCallback(pWd, key_callback);

  // resize callback
  glfwSetFramebufferSizeCallback(pWd, resize_callback);

  // make opengl context
  glfwMakeContextCurrent(pWd);

  // initialize gl
  if (gladLoadGL() == 0) {
    LOGE("Load OpenGL failed!");
    return -1;
  }
  LOGI("OpenGL version:{}.{}", GLVersion.major, GLVersion.minor);

  // create shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &glsl::vs, nullptr);
  glCompileShader(vertex_shader);
  if (!check_compile(vertex_shader, glsl::vs)) {
    return -1;
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &glsl::fs, nullptr);
  glCompileShader(fragment_shader);
  if (!check_compile(fragment_shader, glsl::fs)) {
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
  GLuint position_buffer = 0;
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(position), position.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  GLuint color_buffer = 0;
  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);

  // create ebo
  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

  // unbind vao
  glBindVertexArray(0);

  // set viewport
  int height = 0;
  int width = 0;
  glfwGetFramebufferSize(pWd, &width, &height);
  glViewport(0, 0, width, height);

  // buffer swapping setting
  glfwSwapInterval(2);

  // clear color
  glClearColor(COLOR(color::whitesmoke), 1.0F);

  // set draw type
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // running until exit
  while (glfwWindowShouldClose(pWd) == 0) {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    const auto loc = glGetUniformLocation(shader_program, "color_scalar");
    const auto scalar = std::clamp(static_cast<float>(sin(glfwGetTime())) + 0.5F, 0.2F, 0.9F);
    glUniform1f(loc, scalar);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);

    // event loop
    glfwPollEvents();

    // display
    glfwSwapBuffers(pWd);
  }

  LOGI("user request to close this window!");

  // delete vertex array object and vertex buffer object
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &position_buffer);

  // destroy window
  glfwDestroyWindow(pWd);

  // terminate
  glfwTerminate();
}

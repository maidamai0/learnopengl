#include <cstdlib>

#include "common/glfw_helpper.h"
#include "common/log.h"
#include "common/shader.h"
#include "common/texture.h"
#include "common/win_main.h"
#include "textures_fs.glsl.h"
#include "textures_vs.glsl.h"

namespace {
float g_texture_ratio = 0.5;
}

// triangle point
// clang-format off
// float vertices[] = {
//     // positions          // colors           // texture coords
//      0.5F,  0.5F, 0.0F,   1.0F, 0.0F, 0.0F,   0.55f, 0.55f,   // top right
//      0.5F, -0.5F, 0.0F,   0.0F, 1.0F, 0.0F,   0.55f, 0.45f,   // bottom right
//     -0.5F, -0.5F, 0.0F,   0.0F, 0.0F, 1.0F,   0.45f, 0.45f,   // bottom left
//     -0.5F,  0.5F, 0.0F,   1.0F, 1.0F, 0.0F,   0.45f, 0.55f    // top left 
// };

float vertices[] = {
    // positions          // colors           // texture coords
     0.5F,  0.5F, 0.0F,   1.0F, 0.0F, 0.0F,   1.0F, 1.0F,   // top right
     0.5F, -0.5F, 0.0F,   0.0F, 1.0F, 0.0F,   1.0F, 0.0F,   // bottom right
    -0.5F, -0.5F, 0.0F,   0.0F, 0.0F, 1.0F,   0.0F, 0.0F,   // bottom left
    -0.5F,  0.5F, 0.0F,   1.0F, 1.0F, 0.0F,   0.0F, 1.0F    // top left 
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

// clang-format on

void key_callback_ratio(GLFWwindow *window, int key, int scan_code, int action, int mods) {
  (void)scan_code;
  (void)mods;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    LOGI("Escape pressed");
    glfwSetWindowShouldClose(window, GLFW_TRUE);  // not work
  } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    LOGI("GLFW_KEY_UP pressed");
    g_texture_ratio += 0.1F;

    if (g_texture_ratio > 1.0F) {
      g_texture_ratio = 1.0F;
    }
  } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    LOGI("GLFW_KEY_DOWN pressed");
    g_texture_ratio -= 0.1F;

    if (g_texture_ratio < 0.0F) {
      g_texture_ratio = 0.0F;
    }
  }
}

auto main(int argc, char **argv) -> int {
  (void)argc;
  (void)argv;

  // set error callback
  glfwSetErrorCallback(err_callback);

  GLFW_GUARD;

  // create a window
  auto *pWd = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
  if (!pWd) {
    LOGE("create window failed!");
    return EXIT_FAILURE;
  }

  // set key callback
  glfwSetKeyCallback(pWd, key_callback_ratio);

  // resize callback
  glfwSetFramebufferSizeCallback(pWd, resize_callback);

  // make opengl context
  glfwMakeContextCurrent(pWd);

  // initialize gl
  if (!gladLoadGL()) {
    LOGE("Load OpenGL failed!");
    return EXIT_FAILURE;
  }
  LOGI("OpenGL version:{}.{}", GLVersion.major, GLVersion.minor);

  // get gl info
  LOGI("rederer is {}", glGetString(GL_RENDERER));
  LOGI("version is {}", glGetString(GL_VERSION));

  // vao
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // copy data to graphical card with vbo
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // EBO
  GLuint ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  Shader shader(glsl::textures_vs, glsl::textures_fs);

  // position
  const auto pos_location = glGetAttribLocation(shader.GetProgram(), "vPos");
  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
  glEnableVertexAttribArray(pos_location);

  // color
  const auto color_location = glGetAttribLocation(shader.GetProgram(), "vCol");
  glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(color_location);

  // texture cooridinatin
  const auto tex_location = glGetAttribLocation(shader.GetProgram(), "vTex");
  glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(tex_location);

  // texture
  Texture texture("winnie_the_pooh.jpg");
  Texture texture1("awesomeface.png");

  glUniform1i(glGetUniformLocation(shader.GetProgram(), "outTexture"), 0);
  glUniform1i(glGetUniformLocation(shader.GetProgram(), "outTexture1"), 1);

  // clear color
  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

  // running until exit
  while (!glfwWindowShouldClose(pWd)) {
    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup environment
    glBindVertexArray(vao);

    // draw
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1.GetTextureID());

    // set ratio
    glUniform1f(glGetUniformLocation(shader.GetProgram(), "ratio"), g_texture_ratio);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // poll event
    glfwWaitEvents();

    // display
    glfwSwapBuffers(pWd);
  }

  LOGI("user request to close this window!");

  // destroy window
  glfwDestroyWindow(pWd);

  // terminate
  glfwTerminate();
}

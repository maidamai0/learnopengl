/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief
 * @version 0.1
 * @date 2019-08-28
 *
 */

#include <iostream>
#include <ostream>

#include "common/color.h"
#include "common/glfw_helpper.h"
#include "common/log.h"
#include "common/win_main.h"

auto main(int argc, char **argv) -> int {
  GLFW_GUARD;

  // set error callback
  glfwSetErrorCallback(err_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create a window
  auto *pWd = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
  if (pWd == nullptr) {
    LOGE("create window failed!");
  }
  LOGI("OpenGL version got:{}", glfwGetVersionString());

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

  // set viewport
  int height = 0;
  int width = 0;
  glfwGetFramebufferSize(pWd, &width, &height);
  glViewport(0, 0, width, height);

  // buffer swapping setting
  glfwSwapInterval(1);

  glClearColor(COLOR(color::linen), 1.0F);

  // running until exit
  while (glfwWindowShouldClose(pWd) == 0) {
    // rendering
    glClear(GL_COLOR_BUFFER_BIT);

    // event loop
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

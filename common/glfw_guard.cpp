#include "glfw_guard.h"

GlfwGuard::GlfwGuard(/* args */) {
    if (!glfwInit()) {
        fmt::print("glfw initialize failed\n");
        throw std::exception("glfw initialize failed");
    }

    fmt::print("glfw initialize OK\n");
}

GlfwGuard::~GlfwGuard() {
    glfwTerminate();
    fmt::print("glfw terminated\n");
}
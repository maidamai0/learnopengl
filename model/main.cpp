#include "common/camera.h"
#include "common/glfw_helpper.h"
#include "common/model.h"
#include "common/shader.h"
#include "common/texture.h"
#include "common/win_main.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace {
float g_texture_ratio = 0.5;
float g_x_rotate = -55.0F;
const auto g_screen_width = 1920.0F;
const auto g_screen_height = 1080.0F;
auto g_last_pos_x = 0.0;
auto g_last_pos_y = 0.0;

Camera g_camera({0.0F, 0.0F, 5.0F});

auto g_delta_time = 0.0F;
auto g_last_frame = 0.0F;

// lighting
glm::vec3 g_lightPos(0.2F, 1.0F, 2.0F);

}  // namespace

void key_callback_ratio(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    fmt::print("deltaTime is {}\n", g_delta_time);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        fmt::print("Escape pressed\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE);  // not work
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_UP pressed\n");
        g_texture_ratio += 0.1F;

        if (g_texture_ratio > 1.0F) {
            g_texture_ratio = 1.0F;
        }
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_DOWN pressed\n");
        g_texture_ratio -= 0.1F;

        if (g_texture_ratio < 0.0F) {
            g_texture_ratio = 0.0F;
        }
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_LEFT pressed\n");
        g_x_rotate -= 5.0F;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_RIGHT pressed\n");
        g_x_rotate += 5.0F;
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_W pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kForward, g_delta_time);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_S pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kBackward, g_delta_time);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_A pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kLeft, g_delta_time);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_D pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kRight, g_delta_time);
    }
}

void mouse_callback(GLFWwindow * /*unused*/, double x_pos, double y_pos) {
    static bool first_time = true;
    if (first_time) {
        first_time = false;
        g_last_pos_x = x_pos;
        g_last_pos_y = y_pos;
        return;
    }

    g_camera.ProcessMouseMovement(static_cast<float>((x_pos - g_last_pos_x)),
                                  static_cast<float>((g_last_pos_y - y_pos)),
                                  true);
    g_last_pos_x = x_pos;
    g_last_pos_y = y_pos;
}

void scroll_callback(GLFWwindow * /*unused*/, double /*unused*/, double y_offset) {
    g_camera.ProcessMouseScroll(static_cast<float>(y_offset));
}

auto main(int argc, char **argv) -> int {
    (void)argc;
    (void)argv;

    // set error callback
    glfwSetErrorCallback(err_callback);

    GLFW_GUARD;

    const auto *const mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // create a window
    auto *pWd = glfwCreateWindow(mode->width, mode->height, "load model", nullptr, nullptr);
    if (pWd == nullptr) {
        fmt::print("create window failed!\n");
    }
    fmt::print("window size is {}x{}\n", mode->width, mode->height);

    // set key callback
    glfwSetKeyCallback(pWd, key_callback_ratio);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // glfwSetCursorPosCallback(pWd, mouse_callback);
    glfwSetScrollCallback(pWd, scroll_callback);

    // initialize gl
    if (gladLoadGL() == 0) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // get gl info
    fmt::print("rederer is {}\n", glGetString(GL_RENDERER));
    fmt::print("version is {}\n", glGetString(GL_VERSION));

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    Shader shader("model_vs.glsl", "model_fs.glsl");
    Model render_model("nanosuit.obj");

    // clear color
    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);

    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        auto current_time = static_cast<float>(glfwGetTime());
        g_delta_time = current_time - g_last_frame;
        g_last_frame = current_time;

        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw object
        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.GetZoom()), g_screen_width / g_screen_height, 0.1F, 100.0F);
        glm::mat4 view = g_camera.GetViewMatrix();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0F);
        model = glm::translate(
            model,
            glm::vec3(0.0F, -1.75F, 0.0F));  // translate it down so it's at the center of the scene
        model = glm::scale(
            model,
            glm::vec3(0.2F, 0.2F, 0.2F));  // it's a bit too big for our scene, so scale it down
        shader.SetMat4("model", model);
        render_model.Draw(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(pWd);
        glfwPollEvents();
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

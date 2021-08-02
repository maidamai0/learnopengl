#include <cstdlib>

#include "camera_fs.glsl.h"
#include "camera_vs.glsl.h"
#include "common/camera.h"
#include "common/glfw_helpper.h"
#include "common/log.h"
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

}  // namespace

// triangle point
// clang-format off
float vertices[] = {
    -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,
     0.5F, -0.5F, -0.5F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,

    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
    -0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

    -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
     0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
     0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
     0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
     0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
    -0.5F,  0.5F, -0.5F,  0.0F, 1.0F
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
// clang-format on

auto deltaTime = 0.0F;
auto lastFrame = 0.0F;

void key_callback_ratio(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    LOGI("deltaTime is {}", deltaTime);
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
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_LEFT pressed");
        g_x_rotate -= 5.0F;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_RIGHT pressed");
        g_x_rotate += 5.0F;
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_W pressed");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kForward, deltaTime);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_S pressed");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kBackward, deltaTime);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_A pressed");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kLeft, deltaTime);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        LOGI("GLFW_KEY_D pressed");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kRight, deltaTime);
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

    // capture cursor
    glfwSetInputMode(pWd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(pWd, mouse_callback);
    glfwSetScrollCallback(pWd, scroll_callback);

    // initialize gl
    if (!gladLoadGL()) {
        LOGE("Load OpenGL failed!");
        return EXIT_FAILURE;
    }
    LOGI("OpenGL version:{}.{}", GLVersion.major, GLVersion.minor);

    // get gl info
    LOGI("rederer is {}", glGetString(GL_RENDERER));
    LOGI("version is {}", glGetString(GL_VERSION));

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

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

    Shader shader(glsl::camera_vs, glsl::camera_fs);

    // position
    const auto pos_location = glGetAttribLocation(shader.GetProgram(), "aPos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(pos_location);

    // texture cooridinatin
    const auto tex_location = glGetAttribLocation(shader.GetProgram(), "aTexCoord");
    glVertexAttribPointer(
        tex_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);

    // texture
    Texture texture("winnie_the_pooh.jpg");
    Texture texture1("awesomeface.png");

    glUniform1i(glGetUniformLocation(shader.GetProgram(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "texture2"), 1);

    // clear color
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

    // orientation
    std::vector<glm::vec3> cubePositions{glm::vec3(0.0F, 0.0F, 0.0F),
                                         glm::vec3(2.0F, 5.0F, -15.0F),
                                         glm::vec3(-1.5F, -2.2F, -2.5F),
                                         glm::vec3(-3.8F, -2.0F, -12.3F),
                                         glm::vec3(2.4F, -0.4F, -3.5F),
                                         glm::vec3(-1.7F, 3.0F, -7.5F),
                                         glm::vec3(1.3F, -2.0F, -2.5F),
                                         glm::vec3(1.5F, 2.0F, -2.5F),
                                         glm::vec3(1.5F, 0.2F, -1.5F),
                                         glm::vec3(-1.3F, 1.0F, -1.5F)};

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup environment
        glBindVertexArray(vao);

        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;

        // draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1.GetTextureID());

        // set ratio
        glUniform1f(glGetUniformLocation(shader.GetProgram(), "ratio"), g_texture_ratio);

        // view transformation
        // float radius = 10.0F;
        // float camX = static_cast<float>(sin(glfwGetTime())) * radius;
        // float camZ = static_cast<float>(cos(glfwGetTime())) * radius;
        // glm::mat4 view = glm::lookAt(
        //     glm::vec3(camX, 0.0, camZ), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 1.0F,
        //     0.0F));

        glm::mat4 view = g_camera.GetViewMatrix();
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // projection transformation
        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.GetZoom()), g_screen_width / g_screen_height, 0.1F, 100.0F);
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection));

        // model transformation
        glBindVertexArray(vao);
        for (auto &position : cubePositions) {
            glm::mat4 model = glm::mat4(1.0F);
            model = glm::translate(model, position);
            model = glm::rotate(model,
                                (float)glfwGetTime() * glm::radians(g_x_rotate),
                                glm::vec3(1.0F, 0.3F, 0.5F));
            glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"),
                               1,
                               GL_FALSE,
                               glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // view transformation
        auto view1 = glm::translate(glm::mat4(1.0F), glm::vec3(1.5F, 0.0F, -10.0F));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view1));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // view transformation
        auto view2 = glm::translate(glm::mat4(1.0F), glm::vec3(10.0F, 0.0F, -20.0F));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view2));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(pWd);
        glfwWaitEvents();
    }

    LOGI("user request to close this window!");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

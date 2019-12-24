#include "common/camera.h"
#include "common/glfw_helpper.h"
#include "common/shader.h"
#include "common/texture.h"
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

Camera g_camera({0.0f, 0.0f, 5.0f});

auto g_delta_time = 0.0F;
auto g_last_frame = 0.0F;

// lighting
glm::vec3 g_lightPos(0.2f, 1.0f, 2.0f);

}  // namespace

// triangle point
// clang-format off
float vertices[] = {
    -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,
     0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
     0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
     0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
    -0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
    -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 

    -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
     0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
    -0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
    -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,

    -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
    -0.5F,  0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
    -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
    -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
    -0.5F, -0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
    -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,

     0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
     0.5F,  0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
     0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
     0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
     0.5F, -0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
     0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,

    -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
     0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
     0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
     0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
    -0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
    -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,

    -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
     0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
     0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
    -0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
    -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F
};
// clang-format on

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

    auto const mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // create a window
    auto pWd = glfwCreateWindow(mode->width, mode->height, "materials", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
    }
    fmt::print("window size is {}x{}\n", mode->width, mode->height);

    // set key callback
    glfwSetKeyCallback(pWd, key_callback_ratio);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // capture cursor
    // glfwSetInputMode(pWd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glfwSetCursorPosCallback(pWd, mouse_callback);
    glfwSetScrollCallback(pWd, scroll_callback);

    // initialize gl
    if (!gladLoadGL()) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // get gl info
    fmt::print("rederer is {}\n", glGetString(GL_RENDERER));
    fmt::print("version is {}\n", glGetString(GL_VERSION));

    // enable depth testing
    glEnable(GL_DEPTH_TEST);

    // copy data to graphical card with vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // light vao
    GLuint vao_light = 0;
    glGenVertexArrays(1, &vao_light);
    glBindVertexArray(vao_light);

    Shader light_shader("materials_light_vs.glsl", "materials_light_fs.glsl");
    const auto light_pos_location = glGetAttribLocation(light_shader.GetProgram(), "aPos");
    glVertexAttribPointer(light_pos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(light_pos_location);
    light_shader.SetMat4("view", g_camera.GetViewMatrix());

    // object vao
    GLuint vao_object = 0;
    glGenVertexArrays(1, &vao_object);
    glBindVertexArray(vao_object);

    Shader object_shader("materials_vs.glsl", "materials_fs.glsl");
    const auto object_pos_location = glGetAttribLocation(object_shader.GetProgram(), "aPos");
    glVertexAttribPointer(object_pos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(object_pos_location);

    const auto object_normal_location = glGetAttribLocation(object_shader.GetProgram(), "aNormal");
    glVertexAttribPointer(
        object_normal_location, 3, GL_FLOAT, false, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(object_normal_location);
    auto object_color = glm::vec3(1.0f, 0.5f, 0.31f);
    object_shader.SetVec3("material.ambient", object_color);
    object_shader.SetVec3("material.diffuse", object_color);
    object_shader.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    object_shader.SetFloat("material.shininess", 32.0f);
    object_shader.SetMat4(
        "model", glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.0f)));
    object_shader.SetVec3("ViewPos", g_camera.GetPosition());

    // clear color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        auto current_time = static_cast<float>(glfwGetTime());
        g_delta_time = current_time - g_last_frame;
        g_last_frame = current_time;

        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.GetZoom()), g_screen_width / g_screen_height, 0.1f, 100.0f);

        g_lightPos.x = static_cast<float>(sin(glfwGetTime())) * 1.0f;
        g_lightPos.y = static_cast<float>(cos(glfwGetTime())) * 1.0f;
        g_lightPos.z = static_cast<float>(sin(glfwGetTime())) * 2.0f;

        // Draw object
        object_shader.Use();
        object_shader.SetMat4("projection", projection);
        object_shader.SetMat4("view", g_camera.GetViewMatrix());
        glm::vec3 light_color;
        light_color.x = static_cast<float>(sin(glfwGetTime() * 2.0f));
        light_color.y = static_cast<float>(sin(glfwGetTime() * 0.7f));
        light_color.z = static_cast<float>(sin(glfwGetTime() * 1.3f));
        const auto light_ambient_color = light_color * glm::vec3(0.2f);
        const auto light_diffuse_color = light_color * glm::vec3(0.5f);
        object_shader.SetVec3("light.ambient", light_ambient_color);
        object_shader.SetVec3("light.diffuse", light_diffuse_color);
        object_shader.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        object_shader.SetVec3("light.position", g_lightPos);
        glBindVertexArray(vao_object);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw light
        light_shader.Use();
        auto light_model = glm::mat4(1.0f);
        // light_model = glm::rotate(light_model,
        //                           (float)glfwGetTime() * glm::radians(g_x_rotate),
        //                           glm::vec3(0.0f, 10.5f, 0.0));
        light_model = glm::translate(light_model, g_lightPos);
        light_model = glm::scale(light_model, glm::vec3(0.2f));
        light_shader.SetMat4("model", light_model);
        light_shader.SetMat4("projection", projection);
        // light_shader.SetMat4("view", g_camera.GetViewMatrix());
        glBindVertexArray(vao_light);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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

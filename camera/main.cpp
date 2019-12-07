#include "common/camera.h"
#include "common/glfw_helpper.h"
#include "common/shader.h"
#include "common/texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace {
float g_texture_ratio = 0.5;
float g_x_rotate = -55.0f;
const auto g_screen_width = 1920.0f;
const auto g_screen_height = 1080.0f;
auto g_last_pos_x = 0.0;
auto g_last_pos_y = 0.0;

Camera g_camera({0.0f, 0.0f, 5.0f});

}  // namespace

// triangle point
// clang-format off
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};
// clang-format on

auto deltaTime = 0.0f;
auto lastFrame = 0.0f;

void key_callback_ratio(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    fmt::print("deltaTime is {}\n", deltaTime);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        fmt::print("Escape pressed\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE);  // not work
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_UP pressed\n");
        g_texture_ratio += 0.1f;

        if (g_texture_ratio > 1.0f) {
            g_texture_ratio = 1.0f;
        }
    } else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_DOWN pressed\n");
        g_texture_ratio -= 0.1f;

        if (g_texture_ratio < 0.0f) {
            g_texture_ratio = 0.0f;
        }
    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_LEFT pressed\n");
        g_x_rotate -= 5.0f;
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_RIGHT pressed\n");
        g_x_rotate += 5.0f;
    } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_W pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kForward, deltaTime);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_S pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kBackward, deltaTime);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_A pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kLeft, deltaTime);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        fmt::print("GLFW_KEY_D pressed\n");
        g_camera.ProcessKeyBoardEvent(CameraDirection::kRight, deltaTime);
    }
}

void mouse_callback(GLFWwindow *, double x_pos, double y_pos) {
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

void scroll_callback(GLFWwindow *, double, double y_offset) {
    g_camera.ProcessMouseScroll(static_cast<float>(y_offset));
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // set error callback
    glfwSetErrorCallback(err_callback);

    GLFW_GUARD;

    // create a window
    auto pWd = glfwCreateWindow(640, 480, "Camera", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
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
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // get gl info
    fmt::print("rederer is {}\n", glGetString(GL_RENDERER));
    fmt::print("version is {}\n", glGetString(GL_VERSION));

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

    Shader shader("camera_vs.glsl", "camera_fs.glsl");

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // orientation
    std::vector<glm::vec3> cubePositions{glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(2.0f, 5.0f, -15.0f),
                                         glm::vec3(-1.5f, -2.2f, -2.5f),
                                         glm::vec3(-3.8f, -2.0f, -12.3f),
                                         glm::vec3(2.4f, -0.4f, -3.5f),
                                         glm::vec3(-1.7f, 3.0f, -7.5f),
                                         glm::vec3(1.3f, -2.0f, -2.5f),
                                         glm::vec3(1.5f, 2.0f, -2.5f),
                                         glm::vec3(1.5f, 0.2f, -1.5f),
                                         glm::vec3(-1.3f, 1.0f, -1.5f)};

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
        // float radius = 10.0f;
        // float camX = static_cast<float>(sin(glfwGetTime())) * radius;
        // float camZ = static_cast<float>(cos(glfwGetTime())) * radius;
        // glm::mat4 view = glm::lookAt(
        //     glm::vec3(camX, 0.0, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f,
        //     0.0f));

        glm::mat4 view = g_camera.GetViewMatrix();
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // projection transformation
        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.GetZoom()), g_screen_width / g_screen_height, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection));

        // model transformation
        glBindVertexArray(vao);
        for (auto &position : cubePositions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model,
                                (float)glfwGetTime() * glm::radians(g_x_rotate),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"),
                               1,
                               GL_FALSE,
                               glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // view transformation
        auto view1 = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -10.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view1));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // view transformation
        auto view2 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -20.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view2));

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

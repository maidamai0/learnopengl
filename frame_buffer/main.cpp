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

Camera g_camera({0.0F, 0.0F, 5.0F});

auto g_delta_time = 0.0F;
auto g_last_frame = 0.0F;

// lighting
// glm::vec3 g_lightPos(0.2F, 1.0F, 2.0F);

// clang-format off
    float g_cubeVertices[] = {
        // positions          // texture Coords
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
    float g_planeVertices[] = {
        // positions          // texture Coords 
         5.0F, -0.5F,  5.0F,  2.0F, 0.0F,
        -5.0F, -0.5F,  5.0F,  0.0F, 0.0F,
        -5.0F, -0.5F, -5.0F,  0.0F, 2.0F,

         5.0F, -0.5F,  5.0F,  2.0F, 0.0F,
        -5.0F, -0.5F, -5.0F,  0.0F, 2.0F,
         5.0F, -0.5F, -5.0F,  2.0F, 2.0F
    };
    float g_quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0F,  1.0F,  0.0F, 1.0F,
        -1.0F, -1.0F,  0.0F, 0.0F,
         1.0F, -1.0F,  1.0F, 0.0F,

        -1.0F,  1.0F,  0.0F, 1.0F,
         1.0F, -1.0F,  1.0F, 0.0F,
         1.0F,  1.0F,  1.0F, 1.0F
    };
// clang-format on

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

    Shader shader("frame_buffer.vs", "frame_buffer.fs");
    Shader screenShader("frame_buffer_screen.vs", "frame_buffer_screen.fs");

    // clear color
    glClearColor(0.1F, 0.1F, 0.1F, 1.0F);

    // cube VAO
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_cubeVertices), &g_cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    // plane VAO
    unsigned int planeVAO = 0;
    unsigned int planeVBO = 0;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_planeVertices), &g_planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    // screen quad VAO
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quadVertices), &g_quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    Texture cube_texture("marble.jpg");
    Texture floor_texture("metal.png");

    shader.Use();
    shader.SetInt("texture1", 0);

    screenShader.Use();
    screenShader.SetInt("screenTexture", 0);

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer = 0;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer = 0;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 static_cast<GLsizei>(g_screen_width),
                 static_cast<GLsizei>(g_screen_height),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo = 0;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        static_cast<GLsizei>(g_screen_width),
        static_cast<GLsizei>(g_screen_height));  // use a single renderbuffer object for both a
                                                 // depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              rbo);  // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it
    // is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { {
        fmt::print("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
}
}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        auto current_time = static_cast<float>(glfwGetTime());
        g_delta_time = current_time - g_last_frame;
        g_last_frame = current_time;

        // render
        // ------
        // bind to framebuffer and draw scene as we normally would to color texture
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(
            GL_DEPTH_TEST);  // enable depth testing (is disabled for rendering screen-space quad)

        // make sure we clear the framebuffer's content
        glClearColor(0.1F, 0.1F, 0.1F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        glm::mat4 model = glm::mat4(1.0F);
        glm::mat4 view = g_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.GetZoom()), g_screen_width / g_screen_height, 0.1F, 100.0F);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_texture.GetTextureID());
        model = glm::translate(model, glm::vec3(-1.0F, 0.0F, -1.0F));
        shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0F);
        model = glm::translate(model, glm::vec3(2.0F, 0.0F, 0.0F));
        shader.SetMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floor_texture.GetTextureID());
        shader.SetMat4("model", glm::mat4(1.0F));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer
        // color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);  // disable depth test so screen-space quad isn't discarded due to
                                   // depth test.
        // clear all relevant buffers
        glClearColor(
            1.0F, 1.0F, 1.0F, 1.0F);  // set clear color to white (not really necessery actually,
                                      // since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.Use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);  // use the color attachment texture as
                                                           // the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(pWd);
        glfwPollEvents();
    }

    fmt::print("user request to close this window!\n");

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &quadVBO);

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

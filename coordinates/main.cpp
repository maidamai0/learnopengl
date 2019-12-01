#include "common/glfw_helpper.h"
#include "common/shader.h"
#include "common/texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace {
float g_texture_ratio = 0.5;
float g_x_rotate = -55.0f;
}  // namespace

// triangle point
// clang-format off
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
    }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // set error callback
    glfwSetErrorCallback(err_callback);

    GLFW_GUARD;

    // create a window
    auto pWd = glfwCreateWindow(640, 480, "textures", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback_ratio);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if (!gladLoadGL()) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // get gl info
    fmt::print("rederer is {}\n", glGetString(GL_RENDERER));
    fmt::print("version is {}\n", glGetString(GL_VERSION));

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

    Shader shader("coordinates_vs.glsl", "coordinates_fs.glsl");

    // position
    const auto pos_location = glGetAttribLocation(shader.GetProgram(), "aPos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(pos_location);

    // color
    const auto color_location = glGetAttribLocation(shader.GetProgram(), "aCol");
    glVertexAttribPointer(
        color_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(color_location);

    // texture cooridinatin
    const auto tex_location = glGetAttribLocation(shader.GetProgram(), "aTexCoord");
    glVertexAttribPointer(
        tex_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);

    // texture
    Texture texture("winnie_the_pooh.jpg");
    Texture texture1("awesomeface.png");

    glUniform1i(glGetUniformLocation(shader.GetProgram(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.GetProgram(), "texture2"), 1);

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

        // model transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(g_x_rotate), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        // view transformation
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-2.0f, 0.0f, -5.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        // projection trandformation
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(projection));

        // render container
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // view transformation
        auto view1 = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -10.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view1));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // view transformation
        auto view2 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, -20.0f));
        glUniformMatrix4fv(
            glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view2));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

#include "common/glfw_helpper.h"
#include "common/shader.h"

#include "stb/stb_image.h"

#include <filesystem>

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
    glfwSetKeyCallback(pWd, key_callback);

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

    // config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    Shader shader("textures_vs.glsl", "textures_fs.glsl");

    // position
    const auto pos_location = glGetAttribLocation(shader.GetProgram(), "vPos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(pos_location);

    // color
    const auto color_location = glGetAttribLocation(shader.GetProgram(), "vCol");
    glVertexAttribPointer(
        color_location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(color_location);

    // texture
    const auto tex_location = glGetAttribLocation(shader.GetProgram(), "vTex");
    glVertexAttribPointer(
        tex_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image
    int width = 0, height = 0, num_of_color_channels = 0;
    unsigned char *image_raw_data =
        stbi_load("winnie_the_pooh.jpg", &width, &height, &num_of_color_channels, 0);

    if (image_raw_data) {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_raw_data);
        glGenerateMipmap(texture);
    } else {
        fmt::print(stderr, "load image failed\n");
        return -1;
    }
    stbi_image_free(image_raw_data);

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup environment
        glBindVertexArray(vao);

        // draw
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // poll event
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}

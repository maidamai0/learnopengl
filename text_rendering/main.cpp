/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief seems an old interface, only works for <= OpenGL3.2, and can not work with CORE_PROFILE
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include <array>
#include <functional>
#include <map>
#include <string>

#include "common/define.h"
#include "common/glfw_helpper.h"
#include "common/guard.h"
#include "common/path.h"
#include "common/shader.h"
#include "common/win_main.h"
#include "dependency/glfw/deps/linmath.h"
#include "fmt/core.h"
#include "fragment.fs.h"
#include "freetype/config/ftheader.h"
#include "freetype/freetype.h"
#include "freetype/ftimage.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_int2.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "vertex.vs.h"

struct Character {
    GLuint texture_id_{0};
    glm::ivec2 size_{};
    glm::ivec2 bearing_{};
    FT_Pos advance_{0};
};

static std::map<GLchar, Character> g_characters;
constexpr int g_width = 1920;
constexpr int g_height = 1080;

[[nodiscard]] auto load_font(std::string &&font_file_path) -> bool {
    FT_Library freetype_library = nullptr;
    FT_Face freetype_face = nullptr;

    Guard gurad([&freetype_library, &freetype_face]() {
        FT_Done_Face(freetype_face);
        FT_Done_FreeType(freetype_library);
    });

    if (FT_Init_FreeType(&freetype_library) != 0) {
        fmt::print("initialize freetype library failed");
        return false;
    }

    if (FT_New_Face(freetype_library, R"(C:\Windows\Fonts\comic.ttf)", 0, &freetype_face) != 0) {
        fmt::print("Load arial font failed");
        return false;
    }

    FT_Set_Pixel_Sizes(freetype_face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; ++c) {
        if (FT_Load_Char(freetype_face, c, FT_LOAD_RENDER) != 0) {
            fmt::print("Load {} failed", c);
            continue;
        }

        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     freetype_face->glyph->bitmap.width,
                     freetype_face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     freetype_face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character{
            texture,
            glm::ivec2(freetype_face->glyph->bitmap.width, freetype_face->glyph->bitmap.rows),
            glm::ivec2(freetype_face->glyph->bitmap_left, freetype_face->glyph->bitmap_top),
            freetype_face->glyph->advance.x};
        g_characters[c] = character;
    }

    return true;
}

auto main(int argc, char **argv) -> int {
    UNUSED(argc);
    UNUSED(argv);

    if (glfwInit() == 0) {
        return -1;
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto *pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (pWd == nullptr) {
        fmt::print("create window failed!\n");
        return -1;
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if (gladLoadGL() == 0) {
        fmt::print("Load OpenGL failed!\n");
        exit(-1);
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // buffer swapping setting
    glfwSwapInterval(1);

    // freetype
    if (!load_font(R"(C:\Windows\Fonts\arial.ttf)")) {
        return -1;
    }

    // opengl options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, g_width, g_height);

    // load shaders
    Shader shader(glsl::vertex, glsl::fragment);
    glm::mat4 projection =
        glm::ortho(0.0F, static_cast<GLfloat>(g_width), 0.0F, static_cast<GLfloat>(g_height));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(projection));

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const auto render_text =
        [&](std::string &&text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
            shader.Use();
            glUniform3f(
                glGetUniformLocation(shader.GetProgram(), "textColor"), color.r, color.g, color.b);
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao);

            for (const auto &c : text) {
                auto character = g_characters[c];
                auto x_pos = x + character.bearing_.x * scale;
                auto y_pos = y - (character.size_.y - character.bearing_.y) * scale;

                auto width = character.size_.x * scale;
                auto height = character.size_.y * scale;

                std::array<std::array<GLfloat, 4>, 6> vertices = {
                    {{x_pos, y_pos + height, 0.0, 0.0},
                     {x_pos, y_pos, 0.0, 1.0},
                     {x_pos + width, y_pos, 1.0, 1.0},
                     {x_pos, y_pos + height, 0.0, 0.0},
                     {x_pos + width, y_pos, 1.0, 1.0},
                     {x_pos + width, y_pos + height, 1.0, 0.0}}};
                glBindTexture(GL_TEXTURE_2D, character.texture_id_);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                x += (character.advance_ >> 6) * scale;
            }

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        };

    glfwMaximizeWindow(pWd);
    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        // event loop
        glfwPollEvents();

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        render_text("This is sample text", 200, 200, 2.0F, glm::vec3(0.5, 0.8, 0.2));
        render_text("(C) LearnOpenGL.com", 10, 50, 2.0F, glm::vec3(0.3, 0.7, 0.9));

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // destroy glfw
    glfwTerminate();
}

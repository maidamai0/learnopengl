/**
 * @file mesh.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-12-25
 *
 * @copyright Copyright (c) 2019
 *
 */
#include <cstddef>
#include <string>
#include <vector>

#include "common/shader.h"
#include "glm/glm.hpp"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Texcoord;
};

struct Texture {
    unsigned int id;
    std::string type;
};

class Mesh {
   public:
    Mesh(std::vector<Vertex>&& vts, std::vector<unsigned int>&& ids, std::vector<Texture>&& texs)
        : vertices(std::move(vts)), indices(std::move(ids)), textures(std::move(texs)) {
        setup_mesh();
    }
    ~Mesh();
    void Draw(Shader& shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            } else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }

            shader.SetFloat(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

   private:
    void setup_mesh() {
        // create buffers
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vao_);
        glGenBuffers(1, &ebo_);

        // bind buffers
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(
            GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(unsigned int),
                     &indices[0],
                     GL_STATIC_DRAW);

        // enable attribute
        // TODO remove magic number
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(offsetof(Vertex, Texcoord)));

        // unbind
        glBindVertexArray(0);
    }

   public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

   private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
};

Mesh::~Mesh() {}

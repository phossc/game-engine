#include "model.hpp"

Model::Model(const Mesh& mesh) {
    gen_VAO();
    bind_VAO();
    add_VBO(0, 3, mesh.vertex_positions);
    add_EBO(mesh.indices);
}

Model::~Model() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(buffers_.size(), buffers_.data());
}

void Model::bind_VAO() {
    glBindVertexArray(vao_);
}

void Model::draw() {
    bind_VAO();
    glDrawElements(GL_TRIANGLES, indices_, GL_UNSIGNED_INT, 0);
}

void Model::gen_VAO() {
    glGenVertexArrays(1, &vao_);
}

void Model::add_VBO(int location, int dimensions,
                    const std::vector<GLfloat>& vertices) {
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, nullptr);

    buffers_.push_back(vbo);
}

void Model::add_EBO(const std::vector<GLuint>& indices) {
    indices_ = indices.size();
    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);

    buffers_.push_back(ebo);
}
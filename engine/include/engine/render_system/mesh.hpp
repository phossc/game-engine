#ifndef RENDER_SYSTEM_MESH_HPP
#define RENDER_SYSTEM_MESH_HPP

#include <vector>

#include <glad/glad.h>

struct Mesh {
    std::vector<GLfloat> vertex_positions;
    std::vector<GLuint> indices;
};

#endif /* RENDER_SYSTEM_MESH_HPP */
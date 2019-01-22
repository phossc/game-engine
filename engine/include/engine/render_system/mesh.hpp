#ifndef ENGINE_RENDER_SYSTEM_MESH_HPP
#define ENGINE_RENDER_SYSTEM_MESH_HPP

#include "glad/glad.h"

#include <vector>

namespace engine {

struct Mesh {
    std::vector<GLfloat> vertex_positions;
    std::vector<GLuint> indices;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_MESH_HPP */
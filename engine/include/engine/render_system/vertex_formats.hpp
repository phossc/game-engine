#ifndef ENGINE_RENDER_SYSTEM_VERTEX_FORMATS_HPP
#define ENGINE_RENDER_SYSTEM_VERTEX_FORMATS_HPP

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace engine {

struct Vertex_mesh {
    glm::vec3 position;
    glm::vec3 normal;
};

struct Vertex_mesh_colored {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_VERTEX_FORMATS_HPP */
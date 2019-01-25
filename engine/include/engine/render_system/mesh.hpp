#ifndef ENGINE_RENDER_SYSTEM_MESH_HPP
#define ENGINE_RENDER_SYSTEM_MESH_HPP

#include <vector>

namespace engine {

template <typename VertexFormat>
struct Mesh {
    std::vector<VertexFormat> vertices;
};

template <typename VertexFormat, typename IndexType>
struct Mesh_indexed {
    std::vector<VertexFormat> vertices;
    std::vector<IndexType> indices;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_MESH_HPP */
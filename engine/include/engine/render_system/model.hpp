#ifndef ENGINE_RENDER_SYSTEM_MODEL_HPP
#define ENGINE_RENDER_SYSTEM_MODEL_HPP

#include "engine/render_system/mesh.hpp"

namespace engine {

class Model {
public:
    Model() = delete;
    Model(const Mesh& mesh);
    ~Model();

    void bind_VAO();
    void draw();
    int get_indices_count() const { return indices_; }

private:
    void gen_VAO();
    void add_VBO(int location, int dimensions,
                 const std::vector<GLfloat>& vertices);
    void add_EBO(const std::vector<GLuint>& indices);

    GLuint vao_ = 0;
    int indices_ = 0;
    std::vector<GLuint> buffers_;
};

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_MODEL_HPP */
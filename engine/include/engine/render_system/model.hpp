#ifndef RENDER_SYSTEM_MODEL_HPP
#define RENDER_SYSTEM_MODEL_HPP

#include <render_system/mesh.hpp>

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

#endif /* RENDER_SYSTEM_MODEL_HPP */
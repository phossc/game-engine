#ifndef RENDER_SYSTEM_SHADERS_SHADER_PROGRAM_HPP
#define RENDER_SYSTEM_SHADERS_SHADER_PROGRAM_HPP

#include <set>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <render_system/shaders/shader.hpp>
#include <util/noncopyable.hpp>

class Shader_program : private Noncopyable {
public:
    Shader_program();
    ~Shader_program();

    void attach(Shader& shader);
    void detach(Shader& shader);
    void link();

    GLint get_uniform_location(const GLchar* name);

    void load_uniform_vec_4f(GLint location, const glm::vec4& vec);
    void load_uniform_mat_4f(GLint location, const glm::mat4& mat);

    void use_program();
    GLuint location() { return linked_ ? program_ : 0; }

private:
    bool linked_ = false;
    int frag_shader_count_ = 0;

    GLuint program_ = 0;
    std::set<GLuint> attached_shaders_;

    void attach(GLuint shader);
    void detach(GLuint shader);

    static GLint link_status(GLuint program);
};

#endif /* RENDER_SYSTEM_SHADERS_SHADER_PROGRAM_HPP */
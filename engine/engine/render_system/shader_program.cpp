#include "engine/render_system/shader_program.hpp"

#include "engine/core/system.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

namespace engine {

Shader_program::Shader_program() {
    program_ = glCreateProgram();
}

Shader_program::~Shader_program() {
    glUseProgram(0);
    glDeleteProgram(program_);
}

void Shader_program::attach(Shader& shader) {
    if (shader.type() == GL_FRAGMENT_SHADER) {
        ++frag_shader_count_;
    }

    attached_shaders_.insert(shader.location());
    attach(shader.location());
}

void Shader_program::detach(Shader& shader) {
    if (shader.type() == GL_FRAGMENT_SHADER) {
        --frag_shader_count_;
    }

    attached_shaders_.erase(shader.location());
    detach(shader.location());
}

void Shader_program::attach(GLuint shader) {
    glAttachShader(program_, shader);
}

void Shader_program::detach(GLuint shader) {
    glDetachShader(program_, shader);
}

void Shader_program::link() {
    glLinkProgram(program_);

    if (frag_shader_count_ == 0) {
        sys->logger().warn("Shader program does not contain a fragment shader");
    }

    if (link_status(program_) == GL_TRUE) {
        linked_ = true;

        for (auto shader : attached_shaders_) {
            detach(shader);
        }

        attached_shaders_ = std::set<GLuint>{};
    }
}

GLint Shader_program::link_status(GLuint program) {
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    if (link_status == GL_FALSE) {
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> program_error(log_length >= 1 ? log_length : 1);
        glGetProgramInfoLog(program, program_error.size(), NULL,
                            program_error.data());

        sys->logger().error("Shader program linkage failed: {}",
                            program_error.data());
    }

    return link_status;
}

void Shader_program::use_program() {
    glUseProgram(location());
}

GLint Shader_program::get_uniform_location(const GLchar* name) {
    return glGetUniformLocation(program_, name);
}

static GLuint current_program() {
    GLint64 current = 0;
    glGetInteger64v(GL_CURRENT_PROGRAM, &current);
    return static_cast<GLuint>(current);
}

void Shader_program::load_uniform_vec_4f(GLint location, const glm::vec4& vec) {
    if (current_program() != program_) {
        assert(false);
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Shader_program::load_uniform_mat_4f(GLint location, const glm::mat4& mat) {
    if (current_program() != program_) {
        assert(false);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

} // namespace engine
#ifndef RENDER_SYSTEM_SHADERS_SHADER_HPP
#define RENDER_SYSTEM_SHADERS_SHADER_HPP

#include <string>

#include <glad/glad.h>
#include <util/noncopyable.hpp>

class Shader : private Noncopyable {
public:
    Shader(GLenum type, const std::string& filepath);
    ~Shader();

    GLuint location() { return shader_; }
    GLenum type() { return type_; }

private:
    GLuint shader_;
    GLenum type_;

    static std::string load_file(const std::string& filepath);
    static GLuint load_shader(GLenum type, const char* source);
    static GLint compile_status(GLuint shader);
};

#endif /* RENDER_SYSTEM_SHADERS_SHADER_HPP */
#ifndef ENGINE_RENDER_SYSTEM_SHADER_HPP
#define ENGINE_RENDER_SYSTEM_SHADER_HPP

#include "glad/glad.h"

#include <string>

namespace engine {

class Shader {
public:
    Shader(GLenum type, const std::string& filepath);
    Shader(const Shader&) = delete;
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

} // namespace engine

#endif /* ENGINE_RENDER_SYSTEM_SHADER_HPP */
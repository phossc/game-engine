#include "shader.hpp"

#include <fstream>
#include <vector>

#include <engine/core/system.hpp>

using engine::core::sys;

Shader::Shader(GLenum type, const std::string& filepath) : type_{type} {
    auto source = load_file(filepath);
    shader_ = load_shader(type, source.c_str());
}

Shader::~Shader() {
    glDeleteShader(shader_);
}

std::string Shader::load_file(const std::string& filepath) {
    std::ifstream file{filepath, std::ios::in};

    if (!file.is_open()) {
        sys->logger().error("Could not read shader file: {}", filepath);
        return {};
    }

    std::string source;
    std::string curr_line;

    while (!file.eof()) {
        std::getline(file, curr_line);
        source += curr_line + "\n";
    }
    file.close();

    return source;
}

GLuint Shader::load_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    if (compile_status(shader) == GL_FALSE) {
        return 0;
    }

    return shader;
}

GLint Shader::compile_status(GLuint shader) {
    GLint compile_status = GL_FALSE;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> shader_error(log_length >= 1 ? log_length : 1);
        glGetShaderInfoLog(shader, shader_error.size(), NULL,
                           shader_error.data());

        sys->logger().error("Shader compilation failed: {}",
                            shader_error.data());
    }

    return compile_status;
}

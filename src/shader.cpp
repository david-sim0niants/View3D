#include "shader.hpp"

#include <GL/glew.h>

#include <stdexcept>

namespace view3d {

namespace {

GLuint compileShader(GLenum type, const char* src)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (! success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);

        std::string error = "Shader compilation failed: ";
        error += info_log;
        throw std::runtime_error(error);
    }

    return shader;
}

unsigned int linkProgram(const char* vert_src, const char* frag_src)
{
    GLuint vertex_shader = compileShader(GL_VERTEX_SHADER, vert_src);
    GLuint fragment_shader = compileShader(GL_FRAGMENT_SHADER, frag_src);

    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);

    GLint success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (! success) {
        char info_log[512];
        glGetProgramInfoLog(program_id, 512, nullptr, info_log);

        std::string error = "Shader program linking failed: ";
        error += info_log;
        throw std::runtime_error(error);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program_id;
}

} // namespace

Shader::Shader(const char* vert_src, const char* frag_src)
    : program_id(linkProgram(vert_src, frag_src))
{
}

void Shader::use() const
{
    glUseProgram(program_id);
}

void Shader::setUniform(const char* name, float value) const
{
    GLint location = glGetUniformLocation(program_id, name);
    glUniform1f(location, value);
}

void Shader::setUniform(const char* name, const glm::mat3& value) const
{
    GLint location = glGetUniformLocation(program_id, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const char* name, const glm::mat4& value) const
{
    GLint location = glGetUniformLocation(program_id, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const char* name, const glm::vec3& value) const
{
    GLint location = glGetUniformLocation(program_id, name);
    glUniform3fv(location, 1, &value[0]);
}

void Shader::setUniform(const char* name, const glm::vec4& value) const
{
    GLint location = glGetUniformLocation(program_id, name);
    glUniform4fv(location, 1, &value[0]);
}

} // namespace view3d

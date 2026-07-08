#include "shader.hpp"

#include <GL/glew.h>

#include <format>
#include <stdexcept>

namespace view3d {

namespace {

using namespace std::string_literals;

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
        throw std::runtime_error("Shader compilation failed: "s + info_log);
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
        throw std::runtime_error("Shader compilation failed: "s + info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program_id;
}

GLuint getUniformLocation(GLuint program_id, const char* name)
{
    GLint location = glGetUniformLocation(program_id, name);
    if (location == -1)
        throw std::invalid_argument(
            std::format("Uniform {} not found in shader program.", name));
    else
        return location;
}

void checkGLError(const char* msg)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        throw std::runtime_error(std::format("{} GL error: {}", msg, err));
}

} // namespace

Shader::Shader(const char* vert_src, const char* frag_src)
    : id(linkProgram(vert_src, frag_src))
{
}

void Shader::setUniform(const char* name, float value)
{
    glUniform1f(getUniform(name), value);
    checkGLError("glUniform1f");
}

void Shader::setUniform(const char* name, const glm::mat3& value)
{
    glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
    checkGLError("glUniformMatrix3fv");
}

void Shader::setUniform(const char* name, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &value[0][0]);
    checkGLError("glUniformMatrix4fv");
}

void Shader::setUniform(const char* name, const glm::vec3& value)
{
    glUniform3fv(getUniform(name), 1, &value[0]);
    checkGLError("glUniform3fv");
}

void Shader::setUniform(const char* name, const glm::vec4& value)
{
    glUniform4fv(getUniform(name), 1, &value[0]);
    checkGLError("glUniform4fv");
}

void Shader::use() const
{
    static unsigned int curr_id = 0;
    if (curr_id != id) {
        glUseProgram(id);
        checkGLError("glUseProgram");
        curr_id = id;
    }
}

unsigned int Shader::getUniform(const char* name) const
{
    use();
    return getUniformLocation(id, name);
}

} // namespace view3d

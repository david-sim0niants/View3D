#include "buffer.hpp"

#include <GL/glew.h>

#include <format>

namespace view3d {

namespace {

void checkGLError(const char* msg)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        throw std::runtime_error(std::format("{} GL error: {}", msg, err));
}

} // namespace

unsigned int createRawDeviceBuffer(size_t raw_size)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, raw_size, nullptr, GL_STATIC_DRAW);
    return id;
}

void deleteRawDeviceBuffer(size_t raw_size) noexcept
{
    unsigned int raw_size_uint = static_cast<unsigned int>(raw_size);
    glDeleteBuffers(1, &raw_size_uint);
}

void copyToDeviceBuffer(unsigned int id, const void* data, size_t raw_size)
{
    if (0 == id)
        throw std::invalid_argument("Buffer ID is 0.");

    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, raw_size, data);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        throw std::runtime_error("glBufferSubData failed with GL error: " +
                                 std::to_string(err));
    }
}

void copyFromDeviceBuffer(unsigned int id, void* data, size_t raw_size)
{
    if (0 == id)
        throw std::invalid_argument("Buffer ID is 0.");

    glBindBuffer(GL_ARRAY_BUFFER, id);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, raw_size, data);

    checkGLError("glGetBufferSubData");
}

void copyFromDeviceToDeviceBuffer(unsigned int src_id, unsigned int dst_id,
                                  size_t raw_size)
{
    if (0 == src_id || 0 == dst_id)
        throw std::invalid_argument("Source or destination buffer ID is 0.");

    glBindBuffer(GL_COPY_READ_BUFFER, src_id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                        raw_size);

    checkGLError("glCopyBufferSubData");
}

} // namespace view3d

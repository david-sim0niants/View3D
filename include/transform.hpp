#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace view3d {

class Transform {
  public:
    explicit Transform(glm::mat4 transform = glm::mat4(1.0f))
        : matrix(transform)
    {
    }

    inline glm::mat4 getMatrix() const noexcept
    {
        return matrix;
    }

    void setMatrix(const glm::mat4& transform) noexcept
    {
        matrix = transform;
    }

    inline glm::vec3 getPosition() const noexcept
    {
        return matrix[3];
    }

    inline void setPosition(const glm::vec3& position) noexcept
    {
        matrix[3] = glm::vec4(position, 1.0f);
    }

    inline glm::quat getRotation() const noexcept
    {
        return glm::quat_cast(matrix);
    }

    inline void setRotation(const glm::quat& rotation) noexcept
    {
        glm::vec3 position = getPosition();
        matrix = glm::mat4_cast(rotation);
        matrix[3] = glm::vec4(position, 1.0f);
    }

    void translate(const glm::vec3& translation) noexcept
    {
        matrix = glm::translate(matrix, translation);
    }

    void rotate(float angle, const glm::vec3& axis) noexcept
    {
        matrix = glm::rotate(matrix, angle, axis);
    }

    void scale(const glm::vec3& scaling) noexcept
    {
        matrix = glm::scale(matrix, scaling);
    }

  private:
    glm::mat4 matrix;
};

} // namespace view3d

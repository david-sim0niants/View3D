#pragma once

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

  private:
    glm::mat4 matrix;
};

} // namespace view3d

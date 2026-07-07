#pragma once

#include <glm/glm.hpp>

#include "mesh.hpp"

namespace view3d {

using ObjectID = unsigned long long;

class Object {
  public:
    explicit Object(Mesh3D& mesh, glm::mat4 transform = {0.0f})
        : mesh(mesh), transform(transform)
    {
    }

    inline Mesh3D& getMesh() noexcept
    {
        return mesh;
    }

    inline glm::mat4 getTransform() const noexcept
    {
        return transform;
    }

    inline glm::vec3 getPosition() const noexcept
    {
        return transform[3];
    }

    inline void setPosition(const glm::vec3& position) noexcept
    {
        transform[3] = glm::vec4(position, 1.0f);
    }

  private:
    Mesh3D& mesh;
    glm::mat4 transform;
};

} // namespace view3d

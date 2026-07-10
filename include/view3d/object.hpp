#pragma once

#include "mesh.hpp"
#include "shader.hpp"
#include "transform.hpp"

namespace view3d {

class Object : public Transform {
  public:
    explicit Object(Mesh3D& mesh, Shader shader, glm::vec3 color)
        : mesh(&mesh), shader(shader), color(color)
    {
    }

    inline Mesh3D* getMesh() const noexcept
    {
        return mesh;
    }

    inline void setMesh(Mesh3D& new_mesh)
    {
        mesh = &new_mesh;
    }

    inline const Shader& getShader() const noexcept
    {
        return shader;
    }

    inline void setShader(const Shader& new_shader)
    {
        shader = new_shader;
    }

    inline const glm::vec3& getColor() const noexcept
    {
        return color;
    }

    inline void setColor(const glm::vec3& new_color)
    {
        color = new_color;
    }

  private:
    Mesh3D* mesh = nullptr;
    Shader shader;
    glm::vec3 color;
};

} // namespace view3d

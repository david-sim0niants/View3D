#pragma once

#include "transform.hpp"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace view3d {

class Camera : public Transform {
  public:
    explicit Camera(float aspect_ratio) : aspect_ratio(aspect_ratio)
    {
    }

    auto getViewMatrix() const noexcept
    {
        return glm::inverse(getMatrix());
    }

    const auto& getProjectionMatrix() const noexcept
    {
        return projection_matrix;
    }

    float getAspectRatio() const noexcept
    {
        return aspect_ratio;
    }

    void setAspectRatio(float new_aspect_ratio)
    {
        aspect_ratio = new_aspect_ratio;
        updateProjectionMatrix();
    }

    float getFOV() const noexcept
    {
        return fov;
    }

    void setFOV(float new_fov)
    {
        fov = new_fov;
        updateProjectionMatrix();
    }

    float getNearPlane() const noexcept
    {
        return near_plane;
    }

    void setNearPlane(float new_near_plane)
    {
        near_plane = new_near_plane;
        updateProjectionMatrix();
    }

    float getFarPlane() const noexcept
    {
        return far_plane;
    }

    void setFarPlane(float new_far_plane)
    {
        far_plane = new_far_plane;
        updateProjectionMatrix();
    }

  private:
    void updateProjectionMatrix()
    {
        projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio,
                                             near_plane, far_plane);
    }

    float aspect_ratio = 1.0f;
    float fov = 45.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;

    glm::mat4 projection_matrix;
};

} // namespace view3d

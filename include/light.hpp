#pragma once

#include "transform.hpp"

#include <glm/glm.hpp>

namespace view3d {

class Light : public Transform {
  public:
    Light(glm::vec3 color, float intensity) : color(color), intensity(intensity)
    {
    }

    inline const glm::vec3& getColor() const noexcept
    {
        return color;
    }

    inline void setColor(const glm::vec3& new_color)
    {
        color = new_color;
    }

    inline float getIntensity() const noexcept
    {
        return intensity;
    }

    inline void setIntensity(float new_intensity)
    {
        intensity = new_intensity;
    }

  private:
    glm::vec3 color;
    float intensity;
};

} // namespace view3d

#pragma once

#include <glm/glm.hpp>

namespace view3d {

class Shader {
  public:
    Shader(const char* vert_src, const char* frag_src);

    void setUniform(const char* name, float value);
    void setUniform(const char* name, const glm::mat3& value);
    void setUniform(const char* name, const glm::mat4& value);
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::vec4& value);

    unsigned int getID() const noexcept
    {
        return id;
    }

  private:
    void use() const;
    unsigned int getUniform(const char* name) const;

    unsigned int id;
};

} // namespace view3d

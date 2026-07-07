#pragma once

#include <glm/glm.hpp>

namespace view3d {

class Shader {
  public:
    Shader(const char* vert_src, const char* frag_src);

    void use() const;
    void setUniform(const char* name, float value) const;
    void setUniform(const char* name, const glm::mat3& value) const;
    void setUniform(const char* name, const glm::mat4& value) const;
    void setUniform(const char* name, const glm::vec3& value) const;
    void setUniform(const char* name, const glm::vec4& value) const;

  private:
    unsigned int program_id;
};

} // namespace view3d

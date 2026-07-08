#pragma once

#include "camera.hpp"
#include "scene.hpp"

namespace view3d {

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void render(const Scene& scene);

  private:
    void renderGrid(const Camera& camera);

    Shader grid_shader;
    unsigned int dummy_vao;
};

} // namespace view3d

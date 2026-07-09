#pragma once

#include "camera.hpp"
#include "misc/smart_ptr_hash.hpp"
#include "scene.hpp"

namespace view3d {

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void render(const Scene& scene);
    void setViewport(int width, int height);

  private:
    void renderGrid(const Camera& camera);
    void renderObject(const Scene& scen, const Object* object);
    const Mesh3D* uploadAndAddMesh(const Object* object, const Mesh3D* mesh);

    Shader grid_shader;
    unsigned int dummy_vao;

    std::unordered_map<const Object*, const Mesh3D*> dev_mesh_by_obj;
    misc::SmartPtrSet<std::unique_ptr, Mesh3D> device_meshes;
};

} // namespace view3d

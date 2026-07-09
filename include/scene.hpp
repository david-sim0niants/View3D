#pragma once

#include "camera.hpp"
#include "light.hpp"
#include "object.hpp"

#include <memory>
#include <unordered_set>

namespace view3d {

class Scene {
  public:
    Object* addObject(Object* object);
    Object* removeObject(Object* object);

    inline bool hasObject(Object* object) const
    {
        return objects.find(object) != objects.end();
    }

    inline const auto& getObjects() const noexcept
    {
        return objects;
    }

    Camera* getCamera() const noexcept
    {
        return camera;
    }

    void setCamera(Camera* camera)
    {
        this->camera = camera;
    }

    Light* getLight() const noexcept
    {
        return light;
    }

    void setLight(Light* light)
    {
        this->light = light;
    }

  private:
    std::unordered_set<Object*> objects;
    Camera* camera;
    Light* light;
};

} // namespace view3d

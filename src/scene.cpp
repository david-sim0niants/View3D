#include "scene.hpp"

namespace view3d {

bool Scene::destroyObject(Object* object)
{
    auto it = objects.find(object);
    if (it == objects.end())
        return false;

    objects.erase(it);
    return true;
}

Object* Scene::addObject(std::unique_ptr<Object>&& object)
{
    Object* object_ptr = object.get();
    objects.emplace(std::move(object));
    return object_ptr;
}

void Scene::render()
{
    for (const auto& object : objects) {
        // TODO
    }
}

} // namespace view3d

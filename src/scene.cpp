#include "scene.hpp"

namespace view3d {

Object* Scene::addObject(Object* object)
{
    auto [_, inserted] = objects.insert(object);
    return inserted ? object : nullptr;
}

Object* Scene::removeObject(Object* object)
{
    if (hasObject(object)) {
        objects.erase(object);
        return object;
    } else {
        return nullptr;
    }
}

} // namespace view3d

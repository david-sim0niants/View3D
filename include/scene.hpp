#pragma once

#include "misc/smart_ptr_hash.hpp"
#include "object.hpp"

#include <memory>
#include <optional>
#include <unordered_set>

namespace view3d {

class Scene {
  public:
    template<typename... Args>
    Object* createObject(Args&&... args)
    {
        return addObject(std::make_unique<Object>(std::forward<Args>(args)...));
    }

    bool destroyObject(Object* object);

    Object* includeObject(Object&& obj)
    {
        return createObject(std::move(obj));
    }

    std::optional<Object> releaseObject(Object* object)
    {
        if (hasObject(object)) {
            Object obj = std::move(*object);
            destroyObject(object);
            return obj;
        } else {
            return std::nullopt;
        }
    }

    inline bool hasObject(Object* object) const
    {
        return objects.find(object) != objects.end();
    }

    void render();

  private:
    Object* addObject(std::unique_ptr<Object>&& object);

    misc::SmartPtrSet<std::unique_ptr, Object> objects;
};

} // namespace view3d

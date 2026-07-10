#pragma once

#include "object.hpp"
#include <memory_resource>
#include <unordered_set>

namespace view3d {

template<typename T>
class ResourcesOfType {
  public:
    template<typename... Args>
    T* create(Args&&... args)
    {
        T* resource =
            allocator.template new_object<T>(std::forward<Args>(args)...);
        resources.insert(resource);
        return resource;
    }

    void destroy(T* resource)
    {
        resources.erase(resource);
        allocator.delete_object(resource);
    }

  private:
    std::unordered_set<T*> resources;
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::polymorphic_allocator<T> allocator{&pool};
};

template<typename... T>
class Resources : ResourcesOfType<T>... {
  public:
    template<typename R, typename... Args>
    R* create(Args&&... args)
    {
        return ResourcesOfType<R>::create(std::forward<Args>(args)...);
    }

    template<typename R>
    void destroy(R* resource)
    {
        ResourcesOfType<R>::destroy(resource);
    }
};

template<typename T>
ResourcesOfType<T>& getResourcesOfType()
{
    thread_local static ResourcesOfType<T> resources;
    return resources;
}

template<typename T, typename... Args>
T* createResource(Args&&... args)
{
    return getResourcesOfType<T>().create(std::forward<Args>(args)...);
}

template<typename T>
void destroyResource(T* resource)
{
    getResourcesOfType<T>().destroy(resource);
}

} // namespace view3d

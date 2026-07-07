#pragma once

#include <glm/ext/vector_float3.hpp>
#include <vector>

namespace view3d {

template<typename T, int N>
using Vertex = glm::vec<N, T>;

template<typename T, int N>
class Mesh {
  public:
    using Vertex = view3d::Vertex<T, N>;

    Mesh() = default;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
        : vertices(std::move(vertices)), indices(std::move(indices))
    {
    }

    const auto& getVertices() const noexcept
    {
        return vertices;
    }

    const auto& getIndices() const noexcept
    {
        return indices;
    }

  private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

using Mesh3D = Mesh<float, 3>;

}; // namespace view3d

#pragma once

#include <glm/glm.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace view3d {

template<typename T, int N>
class Mesh {
  public:
    using Vec = glm::vec<N, T>;

    Mesh() = default;

    Mesh(std::vector<Vec>&& vertices, std::vector<unsigned int>&& indices)
        : vertices(std::move(vertices)), indices(std::move(indices))
    {
        validate();
        computeNormals();
    }

    const auto& getVertices() const noexcept
    {
        return vertices;
    }

    const auto& getIndices() const noexcept
    {
        return indices;
    }

    const auto& getNormals() const noexcept
    {
        return normals;
    }

  private:
    void validate()
    {
        if (indices.empty())
            return;

        if (indices.size() % 3 != 0)
            throw std::invalid_argument(
                "Indices size must be a multiple of 3 for triangles.");

        if (*std::max_element(indices.begin(), indices.end()) >= vertices.size())
            throw std::out_of_range(
                "Index value exceeds the number of vertices.");
    }

    void computeNormals()
    {
        normals.resize(vertices.size(), Vec(0.0f));

        for (size_t i = 0; i < indices.size(); i += 3) {
            const auto& v0 = vertices[indices[i]];
            const auto& v1 = vertices[indices[i + 1]];
            const auto& v2 = vertices[indices[i + 2]];

            Vec face_normal = glm::cross(v1 - v0, v2 - v0);

            normals[indices[i]] += face_normal;
            normals[indices[i + 1]] += face_normal;
            normals[indices[i + 2]] += face_normal;
        }

        for (auto& normal : normals)
            normal = glm::normalize(normal);
    }

    std::vector<Vec> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vec> normals;
};

using Mesh3D = Mesh<float, 3>;

}; // namespace view3d

#pragma once

#include <glm/glm.hpp>

#include "buffer.hpp"

#include <span>
#include <stdexcept>

namespace view3d {

template<int N, typename T = float>
struct Vertex {
    glm::vec<N, T> position;
    glm::vec<N, T> normal;
};

template<int N>
    requires(N > 0 && N <= 4)
struct Face {
    unsigned int indices[N];
};

template<typename T, int N>
void computeNormals(std::span<Vertex<N, T>> vertices, std::span<Face<3>> faces)
{
    using Vec = glm::vec<N, T>;

    for (auto& vertex : vertices)
        vertex.normal = Vec(0.0f);

    for (const auto& face : faces) {
        assert(face.indices[0] < vertices.size());
        assert(face.indices[1] < vertices.size());
        assert(face.indices[2] < vertices.size());

        auto& v0 = vertices[face.indices[0]];
        auto& v1 = vertices[face.indices[1]];
        auto& v2 = vertices[face.indices[2]];

        Vec face_normal =
            glm::cross(v1.position - v0.position, v2.position - v0.position);

        v0.normal += face_normal;
        v1.normal += face_normal;
        v2.normal += face_normal;
    }

    for (auto& vertex : vertices)
        vertex.normal = glm::normalize(vertex.normal);
}

template<typename T, int N, int FN = 3>
class Mesh {
  public:
    using Vec = glm::vec<N, T>;
    using Vertex = view3d::Vertex<N, T>;
    using Face = view3d::Face<FN>;

    Mesh() = default;

    Mesh(HostBuffer<Vertex>&& vertices, HostBuffer<Face>&& faces)
        : vertices(std::move(vertices)), faces(std::move(faces))
    {
    }

    Mesh(DeviceBuffer<Vertex>&& vertices, DeviceBuffer<Face>&& faces)
        : vertices(std::move(vertices)), faces(std::move(faces))
    {
    }

    auto&& getVertices(this auto&& self) noexcept
    {
        return self.vertices;
    }

    auto&& getFaces(this auto&& self) noexcept
    {
        return self.faces;
    }

    void computeNormals()
        requires(FN == 3)
    {
        if (! (vertices.inHost() && faces.inHost()))
            throw std::runtime_error(
                "Vertices and faces must be in the host memory.");

        auto& vertices = this->vertices.asHost();
        auto& faces = this->faces.asHost();

        view3d::computeNormals(std::span(vertices), std::span(faces));
    }

  private:
    Buffer<Vertex> vertices;
    Buffer<Face> faces;
};

using Mesh3D = Mesh<float, 3, 3>;

}; // namespace view3d

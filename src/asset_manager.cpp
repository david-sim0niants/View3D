#include "asset_manager.hpp"
#include "platform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <fstream>

namespace view3d {

Mesh3D loadMesh3D(const char* file_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        file_path, aiProcess_Triangulate | aiProcess_GenNormals);

    if (! scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        ! scene->mRootNode) {
        throw std::runtime_error("Assimp load failed: " +
                                 std::string(importer.GetErrorString()));
    }

    const aiMesh* mesh = scene->mMeshes[0];

    HostBuffer<Vertex<3>> vertices(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex<3>& vertex = vertices[i];
        const aiVector3D& ai_vertex = mesh->mVertices[i];

        vertex.position = glm::vec3(ai_vertex.x, ai_vertex.y, ai_vertex.z);
    }

    HostBuffer<Face<3>> faces(mesh->mNumFaces);

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        Face<3>& face = faces[i];
        const aiFace& ai_face = mesh->mFaces[i];

        face.indices[0] = ai_face.mIndices[0];
        face.indices[1] = ai_face.mIndices[1];
        face.indices[2] = ai_face.mIndices[2];
    }

    Mesh3D mesh3d(std::move(vertices), std::move(faces));
    mesh3d.computeNormals();
    return mesh3d;
}

Shader loadShader(const char* vert_fp, const char* frag_fp)
{
    std::ifstream vert_file(vert_fp);
    std::ifstream frag_file(frag_fp);

    if (! vert_file.is_open() || ! frag_file.is_open()) {
        throw std::runtime_error("Failed to open shader files");
    }

    std::string vert_src((std::istreambuf_iterator<char>(vert_file)),
                         std::istreambuf_iterator<char>());
    std::string frag_src((std::istreambuf_iterator<char>(frag_file)),
                         std::istreambuf_iterator<char>());

    return Shader(vert_src.c_str(), frag_src.c_str());
}

Shader loadBuiltinShader(const char* vert_name, const char* frag_name)
{
    auto vert_fp = getSharedDataDirectory() / "shaders" / vert_name;
    auto frag_fp = getSharedDataDirectory() / "shaders" / frag_name;

    return loadShader(vert_fp.string().c_str(), frag_fp.string().c_str());
}

} // namespace view3d

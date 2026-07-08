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

    std::vector<glm::vec3> vertices(mesh->mNumVertices);
    std::transform(
        &mesh->mVertices[0], &mesh->mVertices[mesh->mNumVertices], &vertices[0],
        [](const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); });

    std::vector<unsigned int> indices(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        if (face.mNumIndices != 3) {
            throw std::runtime_error("Non-triangular face found in mesh");
        }
        indices[i * 3 + 0] = face.mIndices[0];
        indices[i * 3 + 1] = face.mIndices[1];
        indices[i * 3 + 2] = face.mIndices[2];
    }

    return Mesh3D(std::move(vertices), std::move(indices));
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

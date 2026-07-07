#pragma once

#include "mesh.hpp"
#include "shader.hpp"

namespace view3d {

inline Mesh3D loadMesh3D(const char* file_path)
{
    // TODO
    return Mesh3D();
}

Shader loadShader(const char* vert_fp, const char* frag_fp);

Shader loadBuiltinShader(const char* vert_name, const char* frag_name);

} // namespace view3d

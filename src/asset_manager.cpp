#include "asset_manager.hpp"
#include "platform.hpp"

#include <fstream>

namespace view3d {

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

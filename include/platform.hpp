#pragma once

#include <filesystem>
namespace view3d {

constexpr std::filesystem::path getSharedDataDirectory()
{
    return VIEW3D_SHARED_DATA_DIR;
}

} // namespace view3d

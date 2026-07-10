#include "platform.hpp"

namespace view3d {

std::filesystem::path getSharedDataDirectory()
{
    return VIEW3D_SHARED_DATA_DIR;
}

} // namespace view3d

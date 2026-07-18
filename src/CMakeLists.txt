option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(TARGET_NAME view3d)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp)

add_library(${TARGET_NAME}
    scene.cpp
    shader.cpp
    buffer.cpp
    asset_manager.cpp
    renderer.cpp
    controller.cpp
    platform.cpp
    view.cpp
)

target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ${ROOT_INCLUDE_DIR}/view3d)

set_target_properties(${TARGET_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC
    glfw OpenGL::GL GLEW::GLEW assimp::assimp)

target_compile_definitions(${TARGET_NAME} PRIVATE
    VIEW3D_SHARED_DATA_DIR="${SHARED_DATA_DIR}")

add_library(View3D::view3d ALIAS ${TARGET_NAME})

if (BUILD_CLI)
    set(TARGET_NAME view3d-cli)

    add_executable(${TARGET_NAME} main.cpp)
    target_link_libraries(${TARGET_NAME} PRIVATE View3D::view3d CLI11::CLI11)
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
        OUTPUT_NAME "view3d")
endif ()

set(TARGET_NAME view3d)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp)

add_executable(${TARGET_NAME} main.cpp
    scene.cpp shader.cpp asset_manager.cpp renderer.cpp controller.cpp)

target_include_directories(${TARGET_NAME} PRIVATE ${ROOT_INCLUDE_DIR})

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(${TARGET_NAME} PRIVATE
    CLI11::CLI11 glfw OpenGL::GL GLEW::GLEW assimp::assimp)

target_compile_definitions(${TARGET_NAME} PRIVATE
    VIEW3D_SHARED_DATA_DIR="${SHARED_DATA_DIR}")

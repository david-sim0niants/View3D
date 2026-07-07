set(TARGET_NAME view3d)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${TARGET_NAME} main.cpp)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(${TARGET_NAME} PRIVATE
    CLI11::CLI11 glfw OpenGL::GL GLEW::GLEW)

#include <CLI/CLI.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_manager.hpp"
#include "scene.hpp"

namespace {

void frameBufferSizeCallback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

int run(const std::string& file_path)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::string title = "View3D - " + file_path;
    GLFWwindow* window =
        glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

    if (! window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() == GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    view3d::Mesh3D mesh = view3d::loadMesh3D(file_path.c_str());

    view3d::Scene scene;
    scene.includeObject(view3d::Object(mesh));

    while (! glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

} // namespace

int main(int argc, char* argv[])
{
    CLI::App app{"View3D - A 3D model viewer application"};

    std::string file_path;
    app.add_option("file", file_path, "Path to the 3D model file")->required();

    if (argc < 2) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    CLI11_PARSE(app, argc, argv);

    std::cout << "Viewing 3D model from file: " << file_path << std::endl;

    return run(file_path);
}

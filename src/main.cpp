#include <CLI/CLI.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_manager.hpp"
#include "controller.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "scene.hpp"

namespace {

using namespace view3d;

class BasicView {
  public:
    explicit BasicView(GLFWwindow* window, const char* file_path)
        : mesh(loadMesh3D(file_path)),
          shader(loadBuiltinShader("basic_vertex.glsl", "basic_fragment.glsl")),
          object(scene.createObject(mesh, shader, glm::vec3{1.0})),
          camera(16.0f / 9.0f), light(glm::vec3(1.0f), 1.0f),
          controller(window, &camera)
    {
        scene.setCamera(&camera);
        scene.setLight(&light);

        camera.translate(glm::vec3(0.0f, 5.0f, 0.0f));

        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        resizeFrame(width, height);
    }

    void update()
    {
        controller.update();
        renderer.render(scene);
    }

  private:
    void resizeFrame(int width, int height)
    {
        glViewport(0, 0, width, height);
        camera.setAspectRatio(static_cast<float>(width) / height);
    }

    static void frameBufferSizeCallback(GLFWwindow* window, int width,
                                        int height)
    {
        void* data = glfwGetWindowUserPointer(window);
        BasicView* basic_view = reinterpret_cast<BasicView*>(data);
        basic_view->resizeFrame(width, height);
    }

    Mesh3D mesh;
    Shader shader;
    Scene scene;
    Object* object;
    Camera camera;
    Light light;
    Renderer renderer;
    Controller controller;
};

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

    glewExperimental = GL_TRUE;
    if (glewInit() == GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    BasicView basic_view(window, file_path.c_str());

    while (! glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        basic_view.update();

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

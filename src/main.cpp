#include <CLI/CLI.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_manager.hpp"
#include "controller.hpp"
#include "light.hpp"
#include "mesh.hpp"
#include "renderer.hpp"
#include "resources.hpp"
#include "scene.hpp"

namespace {

using namespace view3d;

struct View {
    Scene& scene;
    Renderer& renderer;

    void resetFor(GLFWwindow* window)
    {
        int win_width, win_height;
        glfwGetFramebufferSize(window, &win_width, &win_height);
        resizeView(win_width, win_height);
    }

    void resizeView(int width, int height)
    {
        scene.getCamera()->setAspectRatio(static_cast<float>(width) / height);
        renderer.setViewport(width, height);
    }
};

Scene prepareScene(const char* file_path)
{
    Camera* camera = createResource<Camera>(16.0f / 9.0f);
    Light* light = createResource<Light>(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    Mesh3D* mesh = createResource<Mesh3D>(loadMesh3D(file_path));
    Shader shader =
        loadBuiltinShader("basic_vertex.glsl", "basic_fragment.glsl");
    Object* object = createResource<Object>(*mesh, shader, glm::vec3{1.0f});

    Scene scene;
    scene.setCamera(camera);
    scene.setLight(light);
    scene.addObject(object);

    camera->translate(glm::vec3(0.0f, 20.0f, 0.0f));
    light->translate(glm::vec3(20.0f));

    return scene;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    void* data = glfwGetWindowUserPointer(window);
    View* view = reinterpret_cast<View*>(data);
    view->resizeView(width, height);
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

    glewExperimental = GL_TRUE;
    if (glewInit() == GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    Scene scene = prepareScene(file_path.c_str());
    Renderer renderer;
    Controller controller(window, scene.getCamera());

    View view = {scene, renderer};
    view.resetFor(window);
    glfwSetWindowUserPointer(window, &view);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    while (! glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        controller.update();
        renderer.render(scene);

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

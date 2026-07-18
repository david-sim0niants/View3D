#include "view.hpp"

#include "controller.hpp"

namespace view3d {

namespace {

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    void* data = glfwGetWindowUserPointer(window);
    View* view = reinterpret_cast<View*>(data);
    view->resizeView(width, height);
}

} // namespace

View::~View()
{
    glfwTerminate();
}

void View::resizeView(int width, int height)
{
    scene.getCamera()->setAspectRatio(static_cast<float>(width) / height);
    renderer.setViewport(width, height);
}

bool View::view(const char* title, int width, int height)
{
    Controller controller(window, scene.getCamera());

    glfwSetWindowTitle(window, title);
    glfwSetWindowSize(window, width, height);
    glfwShowWindow(window);

    glfwGetFramebufferSize(window, &width, &height);
    resizeView(width, height);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    while (! glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        controller.update();
        renderer.render(scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return true;
}

GLFWwindow* View::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);

    if (! window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    thread_local static bool initialized = false;
    if (! initialized) {
        glewExperimental = GL_TRUE;
        glewInit();
        initialized = true;
    }

    return window;
}

} // namespace view3d

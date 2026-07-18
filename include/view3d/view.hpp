#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "scene.hpp"

namespace view3d {

class View {
  public:
    View() : window(init())
    {
    }

    ~View();

    View(View&&) = delete;
    View(const View&) = delete;
    View& operator=(View&&) = delete;
    View& operator=(const View&) = delete;

    Scene& getScene()
    {
        return scene;
    }

    Renderer& getRenderer()
    {
        return renderer;
    }

    void resizeView(int width, int height);

    bool view(const char* title, int width, int height);

  private:
    GLFWwindow* init();

    GLFWwindow* window;
    Scene scene;
    Renderer renderer;
};

void runWindow(const char* title, int width, int height, View& view);

} // namespace view3d

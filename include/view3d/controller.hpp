#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "transform.hpp"

namespace view3d {

struct ControllerSettings {
    float move_speed = 0.1f;
    float mouse_x_sensitivity = 30.0f;
    float mouse_y_sensitivity = 30.0f;
};

class Controller {
  public:
    Controller(GLFWwindow* window, Transform* target)
        : window(window), target(target)
    {
    }

    void update();

    decltype(auto) getSettings(this auto&& self) noexcept
    {
        return self.settings;
    }

  private:
    GLFWwindow* window;
    Transform* target;

    float yaw = 0.0f;
    float pitch = 0.0f;

    bool right_mouse_was_pressed = false;
    double prev_mouse_x = 0.0f;
    double prev_mouse_y = 0.0f;

    ControllerSettings settings;
};

} // namespace view3d

#include "controller.hpp"

#include <glm/ext.hpp>

namespace view3d {

void Controller::update()
{
    glm::vec3 direction{0};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction.z = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction.z = +1.0f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction.x = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction.x = +1.0f;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        direction.y = -1.0f;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        direction.y = +1.0f;

    target->translate(direction * settings.move_speed);

    // TODO: improve this

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);

        mouse_x /= window_width;
        mouse_y /= window_height;

        if (right_mouse_was_pressed) {
            double delta_x = mouse_x - prev_mouse_x;
            double delta_y = mouse_y - prev_mouse_y;

            yaw += -delta_x * settings.mouse_x_sensitivity;
            pitch += -delta_y * settings.mouse_y_sensitivity;
            pitch = glm::clamp(pitch, -89.99f, 89.99f);

            glm::quat q_yaw =
                glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
            glm::quat q_pitch =
                glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
            glm::quat orientation = q_yaw * q_pitch;

            target->setRotation(orientation);
        } else {
            right_mouse_was_pressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
    } else {
        right_mouse_was_pressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

} // namespace view3d

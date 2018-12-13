#include "../include/input-manager.h"
#include "../include/window.h"
#include <iostream>

bool KeyPattern::operator==(KeyPattern const& other) const {
    return (key == other.key) && (scancode == other.scancode) && (action == other.action) && (mods == other.mods);
}

bool KeyPattern::operator<(KeyPattern const& other) const {
    if (key < other.key) {
        return true;
    } else if (scancode < other.scancode) {
        return true;
    } else if (action < other.action) {
        return true;
    } else if (mods < other.mods) {
        return true;
    } else {
        return false;
    }
}

void InputManager::add_input(KeyPattern pattern, KeyCallback&& callback) {
    key_callbacks.insert({ pattern, callback });
}

void InputManager::press_key(std::optional<int> key, std::optional<int> scancode, std::optional<int> action, std::optional<int> mods) const {
    KeyPattern pattern{key, scancode, action, mods};
    if (key_callbacks.count(pattern)) {
        key_callbacks.at(pattern).callback();
    }
}


void InputManager::setup(GLFWwindow * window_reference) {
    glfwSetKeyCallback(
        window_reference, 
        [] (GLFWwindow* glfw_window, int key, int /*scancode*/, int /*action*/, int /*mods*/) {
            auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
            window->input_manager->press_key(key, {}, {}, {});
        });
}

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <map>
#include <optional>
#include <functional>

class Window;

struct KeyCallback {
    std::optional<std::string> description;
    std::function<void()> callback;
};

struct KeyPattern {
    std::optional<int> key, scancode, action, mods;

    KeyPattern(
        std::optional<int> key = std::nullopt, 
        std::optional<int> scancode = std::nullopt, 
        std::optional<int> action = std::nullopt, 
        std::optional<int> mods = std::nullopt)
        : key{key}, scancode{scancode}, action{action}, mods{mods} {};

    bool operator==(KeyPattern const& other) const;

    bool operator<(KeyPattern const& other) const;
};


struct MousePattern {
    
};

struct MouseCallback {
    
};

class InputManager {
    Window* window_reference;

    public:
    std::map<KeyPattern, KeyCallback> key_callbacks;
    std::map<MousePattern, MouseCallback> mouse_callbacks;

    InputManager() = default;

    void add_input(KeyPattern pattern, KeyCallback&& callback);
    void press_key(std::optional<int> key, std::optional<int> scancode, std::optional<int> action, std::optional<int> mods);
    
};
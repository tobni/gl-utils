#pragma once
#include "../include/input-manager.h"

#include <tuple>
#include <optional>

#include "common.h"


class Window {
    owner<GLFWwindow> window_reference;

    public:

    std::optional<InputManager> input_manager;

    // Window properties
    int width() const;
    int height() const;
    std::tuple<int, int> size() const;
    void set_size(int width, int height);
    
    void set_title(std::string const& title);

    // Framebuffer size
    int framebuffer_width() const;
    int framebuffer_height() const;
    std::tuple<int, int> framebuffer_size() const;

    // Interactions
    void attach(InputManager&& input_manager);
    void close();
    bool should_close();
    void poll();

    Window(owner<GLFWwindow> window_reference);
    Window(Window&& other);
    ~Window();
};

std::optional<Window> make_window(int width, int height, std::string const& name);
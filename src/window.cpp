#include "../include/window.hpp"

std::optional<Window> make_window(int width, int height, const std::string& name) {
    if (glfwInit()) {
        auto * window_reference = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window_reference);
        glfwSwapInterval(1);
        return Window{window_reference};
    }

    return { };
}

void Window::set_size(int width, int height) {
    glfwSetWindowSize(window_reference, width, height);
}


// Window sizes
std::tuple<int, int> Window::size() const {
    int width, height;
    glfwGetWindowSize(window_reference ,&width, &height);
    return { width, height };
}

int Window::width() const {
    return std::get<0>(size());
}

int Window::height() const {
    return std::get<1>(size());
}


std::tuple<int, int> Window::framebuffer_size() const {
    int width, height;
    glfwGetFramebufferSize(window_reference ,&width, &height);
    return { width, height };
}

int Window::framebuffer_width() const {
    return std::get<0>(size());
}

int Window::framebuffer_height() const {
    return std::get<1>(size());
}

void Window::attach(InputManager&& input_manager) {
    this->input_manager = input_manager;
}

bool Window::should_close() {
    return glfwWindowShouldClose(window_reference);
}

void Window::close() {
    glfwSetWindowShouldClose(window_reference, GLFW_TRUE);
}

void Window::poll() {
    glfwSwapBuffers(window_reference);
    glfwPollEvents();
}

Window::Window(owner<GLFWwindow> window_reference)
    : window_reference{window_reference} {
}

Window::Window(Window&& other)     
    : window_reference{other.window_reference}
{
    other.window_reference = nullptr;
}

Window::~Window() {
    glfwDestroyWindow(window_reference);
}
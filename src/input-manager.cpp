#include "../include/input-manager.hpp"


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

void InputManager::press_key(std::optional<int> key, std::optional<int> scancode, std::optional<int> action, std::optional<int> mods) {
    KeyPattern pattern{key, scancode, action, mods};
    if (key_callbacks.count(pattern)) {
        key_callbacks[pattern].callback();
    }
}
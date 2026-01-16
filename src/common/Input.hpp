#pragma once

#include <glm/vec2.hpp>

#include <common/Keys.hpp>

class Input {
public:
    static bool IsKeyPressed(const KeyCode key_code);
    static void PressKey(const KeyCode key_code);
    static void ReleaseKey(const KeyCode key_code);

    static bool IsMouseButtonPressed(const MouseButton mouse_button);
    static void PressMouseButton(const MouseButton mouse_button);
    static void ReleaseMouseButton(const MouseButton mouse_button);

    static glm::vec2 get_mouse_delta() { return m_mouse_delta; }
    static void set_mouse_delta(const glm::vec2& mouse_delta) { m_mouse_delta = mouse_delta; }

private:
    static bool m_keys_pressed[];
    static bool m_mouse_buttons_pressed[];
    static glm::vec2 m_mouse_delta;
};
#pragma once

#include "Window.hpp"

#include <glm/vec2.hpp>

#include <common/Keys.hpp>
#include <common/Input.hpp>
#include <common/Log.hpp>




Window::Window(int width, int height, std::string_view title)
    : m_aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{

    glfw::WindowHints{ .contextVersionMajor = 4,
                        .contextVersionMinor = 6,
                        .openglProfile = glfw::OpenGlProfile::Core }.apply();

    m_window = std::make_unique<glfw::Window>(width, height, title.data());
    glfw::makeContextCurrent(*m_window);


    m_window->framebufferSizeEvent.setCallback(
        [this](glfw::Window&, int w, int h) {
            on_resize(w, h);
        }
    );

    m_window->keyEvent.setCallback(
        [this](glfw::Window& window, glfw::KeyCode key_code, int scan_code, glfw::KeyState state, glfw::ModifierKeyBit modifiers) {
            on_key_press(key_code, scan_code, state, modifiers);
        }
    );

    m_window->mouseButtonEvent.setCallback(
        [this](glfw::Window& window, glfw::MouseButton button, glfw::MouseButtonState state, glfw::ModifierKeyBit modifiers) {
            on_mouse_press(button, state, modifiers);
        }
    );
}

void Window::update()
{
    static glm::vec2 prev_pos;
    static bool first_mouse = true;

    auto [X, Y] = m_window->getCursorPos();

    if (first_mouse) {
        prev_pos = { X,Y };
        first_mouse = false;
    }

    Input::set_mouse_delta({ X - prev_pos.x, Y - prev_pos.y });

    prev_pos = { X, Y };
}

void Window::on_resize(int width, int height)
{
    if (width > 0 && height > 0)
    m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    
    glViewport(0, 0, width, height);
}

void Window::on_key_press(glfw::KeyCode key_code, int scan_code, glfw::KeyState state, glfw::ModifierKeyBit modifiers)
{
    if (state == glfw::KeyState::Press) {
        Input::PressKey(static_cast<KeyCode>(static_cast<int>(key_code)));
    }
    else if (state == glfw::KeyState::Release) {
        Input::ReleaseKey(static_cast<KeyCode>(static_cast<int>(key_code)));
    }
}

void Window::on_mouse_press(glfw::MouseButton button, glfw::MouseButtonState state, glfw::ModifierKeyBit modifiers)
{
    if (state == glfw::MouseButtonState::Press) {
        Input::PressMouseButton(static_cast<MouseButton>(static_cast<int>(button)));
    }
    else if (state == glfw::MouseButtonState::Release) {
        Input::ReleaseMouseButton(static_cast<MouseButton>(static_cast<int>(button)));
    }
}

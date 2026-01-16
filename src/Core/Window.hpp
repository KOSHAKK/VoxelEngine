#pragma once

#include <memory>

#include <glad/gl.h>
#include <glfwpp/glfwpp.h>


class Window
{
public:
	Window(int width, int height, std::string_view title);

	glfw::Window& get_window() { return *m_window; }

	void update();

	float get_aspect() { return m_aspect_ratio; }


private:
	std::unique_ptr<glfw::Window> m_window;
	float m_aspect_ratio;

	void on_resize(int width, int height);
	void on_key_press(glfw::KeyCode key_code, int scan_code, glfw::KeyState state, glfw::ModifierKeyBit modifiers);
	void on_mouse_press(glfw::MouseButton button, glfw::MouseButtonState state, glfw::ModifierKeyBit modifiers);
};

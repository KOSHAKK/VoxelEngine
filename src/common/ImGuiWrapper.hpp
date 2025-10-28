#pragma once

#include <glfwpp/glfwpp.h>

namespace ImGuiWrapper
{
	void init_imgui(glfw::Window& pWindow);

	void update_imgui();

	void destroy_imgui_context();
}
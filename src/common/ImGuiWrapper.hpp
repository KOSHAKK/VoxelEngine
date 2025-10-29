#pragma once

#include <glfwpp/glfwpp.h>

namespace ImGuiWrapper
{
	void init_imgui(glfw::Window& pWindow);

	void update_imgui();

	void destroy_imgui_context();


	inline float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	inline float debug_position[3] = { 0.f };
	inline float debug_scale[3] = { 1.f, 1.f, 1.f };
	inline float debug_rotation[3] = { 0.f, 0.f, 0.f };

}
#pragma once

#include <glfwpp/glfwpp.h>

namespace ImGuiWrapper
{
	void init_imgui(glfw::Window& pWindow);

	void update_imgui();

	void destroy_imgui_context();


	inline float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	inline float debug_light_position[3] = { 5.f, 5.f, -5.f };
	inline float debug_light_scale[3] = { 0.3f, 0.3f, 0.3f };

	inline bool perspective_mode = true;
	inline bool draw_line = false;

	inline float push_strength = 1.f;
}
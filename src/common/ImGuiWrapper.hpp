#pragma once

#include <glfwpp/glfwpp.h>

namespace ImGuiWrapper
{
	void init_imgui(glfw::Window& pWindow);

	void update_imgui();

	void destroy_imgui_context();


	inline float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	inline float debug_block_position[3] = { 1.f, 2.f, -5.f };
	inline float debug_block_scale[3] = { -1.f, -1.f, -1.f };
	inline float debug_block_rotation[3] = { 0.f };

	inline bool perspective_mode = true;
	inline bool draw_line = false;
	inline float debug_camera_position[3] = { 0.f, 4.f, 8.f };
	inline float debug_camera_rotation[3] = { 0.f };

	inline float push_strength = 1.f;
}
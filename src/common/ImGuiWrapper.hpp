#pragma once

#include <glfwpp/glfwpp.h>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace ImGuiWrapper
{
	void init_imgui(glfw::Window& pWindow);

	void update_imgui();

	void destroy_imgui_context();


	inline float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };

	inline bool perspective_mode = true;
	inline bool draw_line = false;

	inline float camera_fov = 120.f;

	inline glm::ivec3 world_size { 1,1,3 };

	inline std::string camera_pos_string;
	inline float camera_speed = 20.f;
	inline glm::ivec2 camera_sensivity = { 100, 100 };
}
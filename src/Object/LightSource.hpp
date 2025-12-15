#pragma once

#include <glm/vec3.hpp>

#include <Object/Block.hpp>

class LightSource
{
public:
	LightSource(const glm::vec3& position = { 0.f, 0.f, 0.f }, const glm::vec3& scale = { 1.f, 1.f, 1.f }, const glm::vec3& color = {120.f, 120.f, 0.f});

	void draw(std::shared_ptr<ShaderProgram> shader_program, const Camera& camera);
	void set_position(const glm::vec3& position);
	void set_scale(const glm::vec3& scale);

private:
	glm::vec3 m_color;
	Block m_cube;
};
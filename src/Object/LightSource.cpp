#include "LightSource.hpp"

LightSource::LightSource(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color)
	: m_color(color)
{
	m_cube.set_position(position);
	m_cube.set_scale(scale);
}

void LightSource::draw(std::shared_ptr<ShaderProgram> shader_program, const Camera& camera)
{
	m_cube.draw(shader_program, camera);
}

void LightSource::set_position(const glm::vec3& position)
{
	m_cube.set_position(position);
}

void LightSource::set_scale(const glm::vec3& scale)
{
	m_cube.set_scale(scale);
}


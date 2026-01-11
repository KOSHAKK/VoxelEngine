#include "World.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <Resources/ResourceManager.hpp>

#include <common/ImGuiWrapper.hpp>
#include <common/Log.hpp>



static inline std::size_t idx(std::size_t x, std::size_t y, std::size_t z, glm::ivec3 world_size) {
	return (x + world_size.x * (y + world_size.y * z));
}



World::World(std::size_t x_size, std::size_t y_size, std::size_t z_size, std::string_view texture_atlas_name)
	: m_world_size(x_size, y_size, z_size),
	  m_chunks(x_size * y_size * z_size),
	  m_meshes(x_size* y_size* z_size),
	  m_texture_atlas_name(texture_atlas_name)
{
	for (std::size_t y = 0; y < m_world_size.y; y++) {
		for (std::size_t z = 0; z < m_world_size.z; z++) {
			for (std::size_t x = 0; x < m_world_size.x; x++) {
				auto index = idx(x, y, z, m_world_size);
				m_chunks[index] = std::make_shared<Chunk>();
				m_chunks[index]->m_pos = { x,y,z };
			}
		}
	}

	for (std::size_t i = 0; i < m_chunks.size(); ++i) {
		auto chunk = m_chunks[i];

		std::vector<std::shared_ptr<Chunk>> closes(27, nullptr);

		for (std::size_t j = 0; j < m_chunks.size(); ++j) {
			auto other = m_chunks[j];

			int ox = other->m_pos.x - chunk->m_pos.x;
			int oy = other->m_pos.y - chunk->m_pos.y;
			int oz = other->m_pos.z - chunk->m_pos.z;

			if (std::abs(ox) > 1 || std::abs(oy) > 1 || std::abs(oz) > 1)
				continue;

			ox++; oy++; oz++; // 0..2
			closes[(oy * 3 + oz) * 3 + ox] = other;
		}

		m_meshes[i] = VoxelMesher::build_mesh(chunk, closes);

	} 
}

void World::draw(const std::shared_ptr<ShaderProgram> shader, const Camera& camera) const
{
	for (std::size_t y = 0; y < m_world_size.y; y++) {
		for (std::size_t z = 0; z < m_world_size.z; z++) {
			for (std::size_t x = 0; x < m_world_size.x; x++) {
				auto index = idx(x, y, z, m_world_size);
				
				glm::vec3 chunkPos = {
					x * Chunk::CHUNK_X,
					y * Chunk::CHUNK_Y,
					z * Chunk::CHUNK_Z
				};
				 
				
				glm::mat4 model_matrix = glm::translate(glm::mat4(1.f), chunkPos);
				
				shader->bind();
				shader->set_matrix4("model", model_matrix);
				shader->set_matrix4("projview", camera.get_projection_matrix() * camera.get_view_matrix());
				ResourceManager::get_texture(m_texture_atlas_name)->bind();



				if (ImGuiWrapper::draw_line) {
					m_meshes[index]->draw(GL_LINES);
				}
				else {
					m_meshes[index]->draw(GL_TRIANGLES);
				}
			}
		}
	}
}



std::shared_ptr<Chunk> World::get_chunk(std::size_t x, std::size_t y, std::size_t z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= m_world_size.x || y >= m_world_size.y|| z >= m_world_size.z) return nullptr;
	return m_chunks[idx(x, y, z, m_world_size)];
}

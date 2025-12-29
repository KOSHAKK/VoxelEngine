#pragma once

#include <vector>
#include <memory>
#include <string>

#include <Voxel/Chunk.hpp>
#include <Voxel/Voxel.hpp>

#include <Object/Mesh.hpp>

#include <Render/VoxelMesher.hpp>
#include <Render/Camera.hpp>


#include <OpenGL/ShaderProgram.hpp>


#include <glm/vec3.hpp>

class World
{
public:
	World(std::size_t x_size, std::size_t y_size, std::size_t z_size, std::string_view texture_atlas_name);

	void draw(const std::shared_ptr<ShaderProgram> shader, const Camera& camera) const;

	std::shared_ptr<Chunk> get_chunk(std::size_t x, std::size_t y, std::size_t z) const;

private:
	std::vector<std::shared_ptr<Chunk>> m_chunks;
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::string m_texture_atlas_name;
	glm::ivec3 m_world_size;
};
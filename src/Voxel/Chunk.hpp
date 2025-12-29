#pragma once

#include <Voxel/Voxel.hpp>
#include <vector>

#include <glm/vec3.hpp>

class Chunk
{
public:
	Chunk();

	std::uint16_t get_id(int x, int y, int z) const;
	bool set_id(int x, int y, int z, std::uint16_t id);



public:
	static constexpr std::size_t CHUNK_X = 16;
	static constexpr std::size_t CHUNK_Y = 16;
	static constexpr std::size_t CHUNK_Z = 16;
	static constexpr std::size_t CHUNK_VOLUME = CHUNK_X * CHUNK_Y * CHUNK_Z;

	glm::ivec3 m_pos;

private:
	std::vector<Voxel> voxels{ CHUNK_VOLUME };


};
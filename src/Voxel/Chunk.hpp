#pragma once

#include <Voxel/Voxel.hpp>
#include <vector>


class Chunk
{
public:
	Chunk();

	std::uint16_t get_id(int x, int y, int z) const;
	bool set_id(int x, int y, int z, std::uint16_t id);


public:
	static constexpr unsigned CHUNK_X = 16;
	static constexpr unsigned CHUNK_Y = 16;
	static constexpr unsigned CHUNK_Z = 16;
	static constexpr unsigned CHUNK_VOLUME = CHUNK_X * CHUNK_Y * CHUNK_Z;

private:
	std::vector<Voxel> voxels{ CHUNK_VOLUME };

};
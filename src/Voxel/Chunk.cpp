#include "Chunk.hpp"

#include <iostream>
#include <cmath>

static inline size_t idx(int x, int y, int z) {
	return static_cast<size_t>((x + Chunk::CHUNK_X * (y + Chunk::CHUNK_Y * z)));
}

Chunk::Chunk()
{
	for (int y = 0; y < CHUNK_Y; y++) {
		for (int z = 0; z < CHUNK_Z; z++) {
			for (int x = 0; x < CHUNK_X; x++) {

				int new_x = (x - CHUNK_X / 2);
				int new_y = (y - CHUNK_X / 2);
				int new_z = (z - CHUNK_X / 2);

				if (sqrt(new_x * new_x + new_y * new_y + new_z * new_z) < CHUNK_X / 2)
				{
					voxels[x + CHUNK_X * (y + CHUNK_Y * z)].id = 1;
				}

			}
		}
	}
}

std::uint16_t Chunk::get_id(int x, int y, int z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z) return 0;
	return voxels[idx(x, y, z)].id;

}

bool Chunk::set_id(int x, int y, int z, std::uint16_t id)
{
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z) return false;

	voxels[idx(x, y, z)].id = id;
	return true;
}

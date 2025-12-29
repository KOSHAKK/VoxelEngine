#pragma once

#include <memory>

#include <Object/Mesh.hpp>
#include <Voxel/Chunk.hpp>



class VoxelMesher
{
public:
	VoxelMesher() = delete;

	static std::shared_ptr<Mesh> build_mesh(std::shared_ptr<Chunk> chunk);
};
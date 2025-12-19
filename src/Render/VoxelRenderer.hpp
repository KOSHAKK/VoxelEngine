#pragma once

#include <memory>

#include <Object/Mesh.hpp>
#include <Voxel/Chunk.hpp>

class VoxelRenderer
{
public:

	std::shared_ptr<Mesh> render(std::shared_ptr<Chunk> chunk);


private:
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer>  m_IBO;
    std::unique_ptr<Mesh>         m_mesh;

};
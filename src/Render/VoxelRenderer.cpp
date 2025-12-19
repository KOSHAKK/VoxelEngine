#include "VoxelRenderer.hpp"

#include <Voxel/Voxel.hpp>

struct ChunkVertex
{
    float x, y, z;  // pos
    float u, v;     // uv
    float l;  // light
};


static inline bool is_in(int x, int y, int z)
{
    return x >= 0 && x < static_cast<int>(Chunk::CHUNK_X) &&
        y >= 0 && y <    static_cast<int>(Chunk::CHUNK_Y) &&
        z >= 0 && z <    static_cast<int>(Chunk::CHUNK_Z);
}

static inline size_t voxel_index(int x, int y, int z)
{
    return static_cast<size_t>((y * static_cast<int>(Chunk::CHUNK_Z + z) * static_cast<int>(Chunk::CHUNK_X + x)));
}

static inline bool is_blocked(std::shared_ptr<Chunk> chunk, int x, int y, int z)
{
    return is_in(x, y, z) && chunk->get_id(x, y, z) != 0;
}

static inline void push_face(std::vector<ChunkVertex>& v,
    std::vector<uint32_t>& i,
    const ChunkVertex& a,
    const ChunkVertex& b,
    const ChunkVertex& c,
    const ChunkVertex& d)
{
    uint32_t base = (uint32_t)v.size();
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    v.push_back(d);

    i.push_back(base + 0);
    i.push_back(base + 1);
    i.push_back(base + 2);
    i.push_back(base + 2);
    i.push_back(base + 3);
    i.push_back(base + 0);
}


std::shared_ptr<Mesh> VoxelRenderer::render(std::shared_ptr<Chunk> chunk)
{
    std::vector<ChunkVertex> verts;
    std::vector<uint32_t> inds;

    constexpr float uvsize = 1.0f / 16.0f;

    for (int y = 0; y < Chunk::CHUNK_Y; y++)
    for (int z = 0; z < Chunk::CHUNK_Z; z++)
    for (int x = 0; x < Chunk::CHUNK_X; x++)
    {
        auto id = chunk->get_id(x, y, z);
        if (!id) continue;

        float u = (id % 16) * uvsize;
        float v = 1.0f - ((1 + id / 16) * uvsize);

        auto makeV = [](float px, float py, float pz, float uu, float vv, float light) {
            return ChunkVertex{ px,py,pz, uu,vv, light};
        };

         
        // TOP (+Y)
        if (!is_blocked(chunk, x, y + 1, z)) {
            float l = 1.0f;
            auto a = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
            auto b = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
            auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
            auto d = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
            push_face(verts, inds, a, b, c, d);
        }

        // BOTTOM (-Y)
        if (!is_blocked(chunk, x, y - 1, z)) {
            float l = 0.75f;
            auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
            auto b = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
            auto c = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
            auto d = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);
            push_face(verts, inds, a, b, c, d);
        }

        // +X
        if (!is_blocked(chunk, x + 1, y, z)) {
            float l = 0.95f;
            auto a = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
            auto b = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
            auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
            auto d = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
            push_face(verts, inds, a, b, c, d);
        }

        // -X
        if (!is_blocked(chunk, x - 1, y, z)) {
            float l = 0.85f;
            auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
            auto b = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
            auto c = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
            auto d = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
            push_face(verts, inds, a, b, c, d);
        }

        // +Z
        if (!is_blocked(chunk, x, y, z + 1)) {
            float l = 0.9f;
            auto a = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
            auto b = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
            auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
            auto d = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
            push_face(verts, inds, a, b, c, d);
        }

        // -Z
        if (!is_blocked(chunk, x, y, z - 1)) {
            float l = 0.8f;
            auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
            auto b = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
            auto c = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
            auto d = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
            push_face(verts, inds, a, b, c, d);
        }
    }

    static BufferLayout chunk_layout = {
        { ShaderDataType::Float3 }, // pos
        { ShaderDataType::Float2 }, // uv
        { ShaderDataType::Float }, // light
    };

    m_VBO = std::make_unique<VertexBuffer>(
        verts.data(),
        verts.size() * sizeof(ChunkVertex),
        chunk_layout,
        VertexBuffer::EUsage::Static
    );

    m_IBO = std::make_unique<IndexBuffer>(
        inds.data(),
        inds.size(),
        VertexBuffer::EUsage::Static
    );
     

     

    return std::make_shared<Mesh>(*m_VBO, *m_IBO);
}

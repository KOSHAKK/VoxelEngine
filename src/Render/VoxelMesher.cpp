#include "VoxelMesher.hpp"

#include <Voxel/Voxel.hpp>

#include <iostream>

#include <common/ImGuiWrapper.hpp>

struct ChunkVertex
{
    float x, y, z;  // pos
    float u, v;     // uv
    float l;        // light
};

static inline int cdiv(int x, int a) { return (x < 0) ? (x / a - 1) : (x / a); }
static inline int local_neg(int x, int size) { return (x < 0) ? (size + x) : x; }
static inline int local(int x, int size) { return (x >= size) ? (x - size) : local_neg(x, size); }


static inline std::shared_ptr<Chunk> get_chunk(
    int x, int y, int z,
    const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    if (z == -1) {
       // std::cout << "123";
    }
    int cx = cdiv(x, Chunk::CHUNK_X) + 1; // 0..2
    int cy = cdiv(y, Chunk::CHUNK_Y) + 1; // 0..2
    int cz = cdiv(z, Chunk::CHUNK_Z) + 1; // 0..2

    int i = cx + 3 * cz + 9 * cy;             // X fastest, then Z, then Y
    return chunks[i];
}

static inline bool is_chunk(int x, int y, int z, const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    auto ch = get_chunk(x, y, z, chunks);
    return ch != nullptr;
}

static inline int voxel(int x, int y, int z, const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    int i = (local(y, Chunk::CHUNK_Y) * Chunk::CHUNK_Z + local(z, Chunk::CHUNK_Z)) * Chunk::CHUNK_X + local(x, Chunk::CHUNK_X);
    auto zz = get_chunk(x, y, z, chunks)->get_voxels()[i].id;
    return zz;
}


static inline bool is_blocked(std::shared_ptr<Chunk> chunk, int x, int y, int z, const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    bool ch = is_chunk(x, y, z, chunks);

    if (ch == false) return false;

    bool vx = voxel(x, y, z, chunks);


        return (!ch || vx);
}

static inline void push_face(std::vector<ChunkVertex>& v,
                             std::vector<uint32_t>& i,
                             const ChunkVertex& a,
                             const ChunkVertex& b,
                             const ChunkVertex& c,
                             const ChunkVertex& d)
{
    uint32_t base = static_cast<std::uint32_t>(v.size());
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


std::shared_ptr<Mesh> VoxelMesher::build_mesh(std::shared_ptr<Chunk> chunk, const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    std::vector<ChunkVertex> verts;
    std::vector<uint32_t> inds;

    //chunks[13] = nullptr;

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
                    return ChunkVertex{ px ,py,pz, uu,vv, light };
                };

                
   


                // TOP (+Y)
                if (!is_blocked(chunk, x, y + 1, z, chunks)) {
                    float l = 1.0f;
                    auto a = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v, l);
                    auto b = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    auto d = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                    push_face(verts, inds, a, b, c, d);
                }

                // BOTTOM (-Y)
                if (!is_blocked(chunk, x, y - 1, z, chunks)) {
                    float l = 0.75f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    auto b = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    auto c = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    auto d = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v + uvsize, l);
                    push_face(verts, inds, a, b, c, d);
                }

                // +X
                if (!is_blocked(chunk, x + 1, y, z, chunks)) {
                    float l = 0.95f;
                    auto a = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + uvsize, v, l);
                    auto b = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u + uvsize, v + uvsize, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    auto d = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    push_face(verts, inds, a, b, c, d);
                }

                // -X
                if (!is_blocked(chunk, x - 1, y, z, chunks)) {
                    float l = 0.85f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    auto b = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    auto c = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    auto d = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u, v + uvsize, l);
                    push_face(verts, inds, a, b, c, d);
                }

                // +Z
                if (!is_blocked(chunk, x, y, z + 1, chunks)) {
                    float l = 0.9f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    auto b = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + uvsize, v, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u + uvsize, v + uvsize, l);
                    auto d = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u, v + uvsize, l);
                    push_face(verts, inds, a, b, c, d);
                }

                // -Z
                if (!is_blocked(chunk, x, y, z - 1, chunks)) {
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

    auto VBO = std::make_shared<VertexBuffer>(
        verts.data(),
        verts.size() * sizeof(ChunkVertex),
        chunk_layout,
        VertexBuffer::EUsage::Static
    );

    auto IBO = std::make_shared<IndexBuffer>(
        inds.data(),
        inds.size(),
        VertexBuffer::EUsage::Static
    );

    return std::make_shared<Mesh>(VBO, IBO);
}
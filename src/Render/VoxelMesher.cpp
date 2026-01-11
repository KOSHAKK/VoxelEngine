#include "VoxelMesher.hpp"

#include <Voxel/Voxel.hpp>

#include <common/ImGuiWrapper.hpp>
#include <common/Log.hpp>



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
    int cx = cdiv(x, Chunk::CHUNK_X) + 1; // 0..2
    int cy = cdiv(y, Chunk::CHUNK_Y) + 1; // 0..2
    int cz = cdiv(z, Chunk::CHUNK_Z) + 1; // 0..2

    if (cx < 0 || cx > 2 || cy < 0 || cy > 2 || cz < 0 || cz > 2)
        return nullptr;

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
    return (ch && voxel(x, y, z, chunks));
}

static inline void push_face(std::vector<ChunkVertex>& v,
                             const ChunkVertex& a,
                             const ChunkVertex& b,
                             const ChunkVertex& c,
                             const ChunkVertex& d)
{
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    
    v.push_back(a);
    v.push_back(c);
    v.push_back(d);
}


std::shared_ptr<Mesh> VoxelMesher::build_mesh(std::shared_ptr<Chunk> chunk, const std::vector<std::shared_ptr<Chunk>>& chunks)
{
    std::vector<ChunkVertex> verts;

    constexpr float UVSIZE = 1.0f / 16.0f;

    constexpr auto makeV = [](float px, float py, float pz, float uu, float vv, float light) {
        return ChunkVertex{ px ,py,pz, uu,vv, light };
    };


    for (int y = 0; y < Chunk::CHUNK_Y; y++)
        for (int z = 0; z < Chunk::CHUNK_Z; z++)
            for (int x = 0; x < Chunk::CHUNK_X; x++)
            {
                auto id = chunk->get_id(x, y, z);
                if (id == 0) continue;

                float u = (id % 16) * UVSIZE;
                float v = 1.0f - ((1 + id / 16) * UVSIZE);


                // TOP (+Y)
                if (!is_blocked(chunk, x, y + 1, z, chunks)) {
                    float l = 1.0f;
                    auto a = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u + UVSIZE, v, l);
                    auto b = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + UVSIZE, v + UVSIZE, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + UVSIZE, l);
                    auto d = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u, v, l);
                    push_face(verts, a, b, c, d);
                }

                // BOTTOM (-Y)
                if (!is_blocked(chunk, x, y - 1, z, chunks)) {
                    float l = 0.75f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    auto b = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + UVSIZE, v, l);
                    auto c = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + UVSIZE, v + UVSIZE, l);
                    auto d = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v + UVSIZE, l);
                    push_face(verts, a, b, c, d);
                }

                // +X
                if (!is_blocked(chunk, x + 1, y, z, chunks)) {
                    float l = 0.95f;
                    auto a = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u + UVSIZE, v, l);
                    auto b = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u + UVSIZE, v + UVSIZE, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u, v + UVSIZE, l);
                    auto d = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    push_face(verts, a, b, c, d);
                }

                // -X
                if (!is_blocked(chunk, x - 1, y, z, chunks)) {
                    float l = 0.85f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    auto b = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u + UVSIZE, v, l);
                    auto c = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u + UVSIZE, v + UVSIZE, l);
                    auto d = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u, v + UVSIZE, l);
                    push_face(verts, a, b, c, d);
                }

                // +Z
                if (!is_blocked(chunk, x, y, z + 1, chunks)) {
                    float l = 0.9f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z + 0.5f, u, v, l);
                    auto b = makeV(x + 0.5f, y - 0.5f, z + 0.5f, u + UVSIZE, v, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z + 0.5f, u + UVSIZE, v + UVSIZE, l);
                    auto d = makeV(x - 0.5f, y + 0.5f, z + 0.5f, u, v + UVSIZE, l);
                    push_face(verts, a, b, c, d);
                }

                // -Z
                if (!is_blocked(chunk, x, y, z - 1, chunks)) {
                    float l = 0.8f;
                    auto a = makeV(x - 0.5f, y - 0.5f, z - 0.5f, u + UVSIZE, v, l);
                    auto b = makeV(x + 0.5f, y - 0.5f, z - 0.5f, u, v, l);
                    auto c = makeV(x + 0.5f, y + 0.5f, z - 0.5f, u, v + UVSIZE, l);
                    auto d = makeV(x - 0.5f, y + 0.5f, z - 0.5f, u + UVSIZE, v + UVSIZE, l);
                    push_face(verts, a, b, c, d);
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

    return std::make_shared<Mesh>(VBO, verts.size());
}
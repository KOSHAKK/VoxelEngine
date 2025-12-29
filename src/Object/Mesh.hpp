#pragma once

#include <OpenGL/VertexArray.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/IndexBuffer.hpp>

#include <memory>

class Mesh
{
public:
    Mesh(std::shared_ptr<VertexBuffer> vertex_buffer,
         std::shared_ptr<IndexBuffer> index_buffer);

    ~Mesh() = default;

    void draw(unsigned int primitive) const;

    size_t get_index_count() const { return m_VAO.get_indices_count(); }

public:
	VertexArray m_VAO;
    std::shared_ptr<VertexBuffer> m_vertex_buffer;
    std::shared_ptr<IndexBuffer> m_index_buffer;
};
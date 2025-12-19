#pragma once

#include <OpenGL/VertexArray.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/IndexBuffer.hpp>


class Mesh
{
public:
    Mesh(const VertexBuffer& vertex_buffer,
         const IndexBuffer&  index_buffer);

    ~Mesh() = default;

    void draw(unsigned int primitive) const;

    size_t get_index_count() const { return m_VAO.get_indices_count(); }

public:
	VertexArray m_VAO;
};
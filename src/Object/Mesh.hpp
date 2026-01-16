#pragma once

#include <OpenGL/VertexArray.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/IndexBuffer.hpp>

#include <memory>

class Mesh
{
public:
    Mesh(std::shared_ptr<VertexBuffer> vertex_buffer, std::size_t vertex_count);

    ~Mesh() = default;

    void draw(unsigned int primitive) const;

public:
    int m_vertex_count;
	VertexArray m_VAO;
    std::shared_ptr<VertexBuffer> m_vertex_buffer;
};
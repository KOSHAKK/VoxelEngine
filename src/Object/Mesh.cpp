#include "Mesh.hpp"

#include <glad/gl.h>

#include <common/Log.hpp>

Mesh::Mesh(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer)
{
	m_VAO.add_vertex_buffer(vertex_buffer);
	m_VAO.set_index_buffer(index_buffer);
}

void Mesh::draw(unsigned int primitive) const
{
    m_VAO.bind();
    glDrawElements(
        primitive,
        static_cast<GLsizei>(m_VAO.get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr
    );
}

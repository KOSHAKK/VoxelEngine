#include "Mesh.hpp"

#include <glad/gl.h>

#include <common/Log.hpp>
#include <common/ImGuiWrapper.hpp>

Mesh::Mesh(std::shared_ptr<VertexBuffer> vertex_buffer, int vertex_count)
    : m_vertex_buffer(vertex_buffer),
      m_vertex_count(vertex_count)
{
    m_VAO.bind();
	m_VAO.add_vertex_buffer(*m_vertex_buffer);
}

void Mesh::draw(unsigned int primitive) const
{
    m_VAO.bind();
    glDrawArrays(primitive, 0, static_cast<GLsizei>(m_vertex_count));
}

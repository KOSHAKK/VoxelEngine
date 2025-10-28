#include "Block.hpp"

#include <glad/glad.h>
#include <common/Log.hpp>

Block::Block(const glm::vec3& position, const glm::vec3& scale)
{
    GLfloat point[] = {
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f 
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f 
    };


    GLuint indices[] = {
        0, 1, 2,
        1, 3, 2
    };

    BufferLayout buffer_layout_1vec3
    {
        ShaderDataType::Float3,
    };


    m_vbo_point = new VertexBuffer(point, sizeof(point), buffer_layout_1vec3);
    m_vbo_color = new VertexBuffer(colors, sizeof(colors), buffer_layout_1vec3);


    m_ebo = new IndexBuffer(indices, 6); // TODO

    m_vao.add_vertex_buffer(*m_vbo_point);
    m_vao.add_vertex_buffer(*m_vbo_color);
    m_vao.set_index_buffer(*m_ebo);


    m_scale_matrix = glm::mat4(scale[0],  0  ,       0,         0,
                               0,         scale[1],  0,         0,
                               0,         0,         scale[2],  0,
                               0,         0,         0,         1
    );

    m_pos_matrix = glm::mat4(1,           0,           0,           0,
                             0,           1,           0,           0,
                             0,           0,           1,           0,
                             position[0], position[1], position[2], 1
    );


}

Block::~Block()
{
	delete m_vbo_point;
	delete m_vbo_color;
	delete m_ebo;
}


void Block::draw(const ShaderProgram& shader) const
{
    shader.bind();

    shader.set_matrix4("scale_matrix", m_scale_matrix);
    shader.set_matrix4("pos_matrix", m_pos_matrix);
    //shader.set_matrix4("rotate_matrix", m_rotate_matrix);



    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_ebo->get_count(), GL_UNSIGNED_INT, nullptr);
}

void Block::set_scale(const glm::vec3& scale)
{
    m_scale_matrix = glm::mat4(scale[0],      0,         0,         0,
                               0,             scale[1],  0,         0,
                               0,             0,         scale[2],  0,
                               0,             0,         0,         1
    );
}

void Block::set_position(const glm::vec3& position)
{
    m_pos_matrix = glm::mat4(1,           0,           0,           0,
                             0,           1,           0,           0,
                             0,           0,           1,           0,
                             position[0], position[1], position[2], 1
    );
}

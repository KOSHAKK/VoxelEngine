#include "Block.hpp"

#include <glad/glad.h>
#include <common/Log.hpp>



Block::Block(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
{
    GLfloat point[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };



    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.5f, 0.2f, 0.8f,
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        4, 0, 3, 3, 7, 4,
        1, 5, 6, 6, 2, 1,
        4, 5, 1, 1, 0, 4,
        3, 2, 6, 6, 7, 3
    };

    BufferLayout buffer_layout_1vec3
    {
        ShaderDataType::Float3,
    };


    m_vbo_point = new VertexBuffer(point, sizeof(point), buffer_layout_1vec3);
    m_vbo_color = new VertexBuffer(colors, sizeof(colors), buffer_layout_1vec3);


    m_ebo = new IndexBuffer(indices, sizeof(indices));

    m_vao.add_vertex_buffer(*m_vbo_point);
    m_vao.add_vertex_buffer(*m_vbo_color);
    m_vao.set_index_buffer(*m_ebo);


	set_scale(scale);
    set_position(position);
	set_rotation(rotation);
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


	glm::mat4 model_matrix = m_pos_matrix * m_rotate_matrix * m_scale_matrix;
    shader.set_matrix4("model_matrix", model_matrix);



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

void Block::set_rotation(const glm::vec3& rotation)
{
    float rotate_x = glm::radians(rotation[0]);
    float rotate_y = glm::radians(rotation[1]);
    float rotate_z = glm::radians(rotation[2]);

    m_rotate_matrix = glm::mat4(1.f);

    m_rotate_matrix *= glm::mat4(1, 0,             0,               0,
                                 0, cos(rotate_x), -sin(rotate_x),  0,
                                 0, sin(rotate_x),  cos(rotate_x),  0,
                                 0, 0,             0,               1
    );

    m_rotate_matrix *= glm::mat4(cos(rotate_y),  0, sin(rotate_y), 0,
                                 0,              1, 0,             0,
                                 -sin(rotate_y), 0, cos(rotate_y), 0,
                                 0,              0, 0,             1
	);

    m_rotate_matrix *= glm::mat4(cos(rotate_z), -sin(rotate_z), 0, 0,
                                 sin(rotate_z),  cos(rotate_z), 0, 0,
                                 0,              0,             1, 0,
                                 0,              0,             0, 1
	);
}

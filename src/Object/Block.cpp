#include "Block.hpp"

#include <glad/glad.h>
#include <common/Log.hpp>

#include <common/ImGuiWrapper.hpp>

#include <Resources/ResourceManager.hpp>

Block::Block(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
{
    GLfloat point[] = {
    -1.0f, -1.f, -1.f,
    -1.0f,  1.f, -1.f,
    -1.0f,  1.f,  1.f,
    -1.0f, -1.f,  1.f,
    
    // BACK            
     1.0f, -1.f, -1.f,
     1.0f,  1.f, -1.f,
     1.0f,  1.f,  1.f,
     1.0f, -1.f,  1.f,
    
     // RIGHT
     -1.0f,  1.f, -1.f,
      1.0f,  1.f, -1.f,
      1.0f,  1.f,  1.f,
     -1.0f,  1.f,  1.f,
    
     // LEFT
     -1.0f, -1.f, -1.f,
      1.0f, -1.f, -1.f,
      1.0f, -1.f,  1.f,
     -1.0f, -1.f,  1.f,
    
     // TOP
     -1.0f, -1.f,  1.f,
     -1.0f,  1.f,  1.f,
      1.0f,  1.f,  1.f,
      1.0f, -1.f,  1.f,
    
      // BOTTOM
      -1.0f, -1.f, -1.f,
      -1.0f,  1.f, -1.f,
       1.0f,  1.f, -1.f,
       1.0f, -1.f, -1.f,
    };

    GLfloat texture_coords[] = {
            0.f, 0.f,
            1.f, 0.f,
            1.f, 1.f,
            0.f, 1.f,
            
            
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            
            
            0.f, 0.f,
            1.f, 0.f,
            1.f, 1.f,
            0.f, 1.f,
            
            
            1.f, 0.f,
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            
            
            0.f, 0.f,
            1.f, 0.f,
            1.f, 1.f,
            0.f, 1.f,
            
            
            0.f, 1.f,
            1.f, 1.f,
            1.f, 0.f,
            0.f, 0.f,

        //           
        //1.0f, 0.0f,
        //0.0f, 1.0f,
        //1.0f, 1.0f,
        //0.5f, 0.2f,
    };

    GLuint indices[] = {
        0,   1,  2,  2,  3,  0, // front
        4,   5,  6,  6,  7,  4, // back
        8,   9, 10, 10, 11,  8, // right
        12, 13, 14, 14, 15, 12, // left
        16, 17, 18, 18, 19, 16, // top
        20, 21, 22, 22, 23, 20  // bottom
    };

    BufferLayout buffer_layout_1vec3
    {
        ShaderDataType::Float3,
    };

    BufferLayout buffer_layout_1vec2
    {
        ShaderDataType::Float2,
    };

    m_vbo_point = new VertexBuffer(point, sizeof(point), buffer_layout_1vec3);
    m_vbo_texture = new VertexBuffer(texture_coords, sizeof(texture_coords), buffer_layout_1vec2);


    m_ebo = new IndexBuffer(indices, sizeof(indices));

    m_vao.add_vertex_buffer(*m_vbo_point);
    m_vao.add_vertex_buffer(*m_vbo_texture);
    m_vao.set_index_buffer(*m_ebo);


	set_scale(scale);
    set_position(position);
	set_rotation(rotation);
}

Block::~Block()
{
	delete m_vbo_point;
	delete m_vbo_texture;
	delete m_ebo;
}


void Block::draw(const std::shared_ptr<ShaderProgram> shader, const Camera& camera) const
{
    shader->bind();


	glm::mat4 model_matrix = m_pos_matrix * m_rotate_matrix * m_scale_matrix;
    glm::mat4 projection_view_matrix = camera.get_projection_matrix() * camera.get_view_matrix();

	glm::mat4 MVP = projection_view_matrix * model_matrix;

    shader->set_matrix4("MVP", MVP);

    

    m_vao.bind();

    glActiveTexture(GL_TEXTURE0);
    ResourceManager::get_texture("stone_brick_texture")->bind();
    shader->set_int("tex", 0);

    if (!ImGuiWrapper::draw_line)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_ebo->get_count()), GL_UNSIGNED_INT, nullptr);
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

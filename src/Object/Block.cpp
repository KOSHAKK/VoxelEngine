#include "Block.hpp"

#include <glad/gl.h>
#include <common/Log.hpp>

#include <common/ImGuiWrapper.hpp>

#include <Resources/ResourceManager.hpp>


Block::Block(const std::string& texture_name, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
    : m_texture_name(texture_name)
{
    m_chunk = std::make_shared<Chunk>();
    m_mesh = VoxelMesher::build_mesh(m_chunk);
    
	set_scale(scale);
    set_position(position);
	set_rotation(rotation);
}

Block::~Block()
{

}

void Block::draw(const std::shared_ptr<ShaderProgram> shader, const Camera& camera) const
{
    glm::mat4 model_matrix = m_pos_matrix * m_rotate_matrix * m_scale_matrix;

    shader->bind();
    shader->set_matrix4("model", model_matrix);
    shader->set_matrix4("projview", camera.get_projection_matrix() * camera.get_view_matrix());
    ResourceManager::get_texture(m_texture_name)->bind();

    if (ImGuiWrapper::draw_line) {
        m_mesh->draw(GL_LINES);
    }
    else {
        m_mesh->draw(GL_TRIANGLES);
    }
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
    m_pos = position;
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

void Block::set_rotation_matrix(const glm::mat4& rot_matrix)
{
    m_rotate_matrix = rot_matrix;
}

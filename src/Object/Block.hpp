#pragma once
#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include <OpenGL/VertexArray.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/IndexBuffer.hpp>
#include <OpenGL/ShaderProgram.hpp>

#include <Render/Camera.hpp>
#include <Render/Texture2D.hpp>


class Block 
{
public:
    Block(const std::string& texture_name = "__[EMPTY]", const glm::vec3& position = {0.f, 0.f, 0.f}, const glm::vec3& scale = {1.f, 1.f, 1.f}, const glm::vec3& rotation = {0.f, 0.f, 0.f});
    ~Block();

    void draw(const std::shared_ptr<ShaderProgram> shader, const Camera& camera, const glm::vec3& color = { 0.f ,0.f ,0.f }) const;

    void set_scale(const glm::vec3& scale);
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);


    void set_rotation_matrix(const glm::mat4& rot_matrix);

private:
    VertexArray m_vao;
    VertexBuffer* m_vbo_point = nullptr;
    VertexBuffer* m_vbo_texture = nullptr;
    IndexBuffer* m_ebo = nullptr;

    std::string m_texture_name;

    glm::mat4 m_pos_matrix;
    glm::mat4 m_scale_matrix;
    glm::mat4 m_rotate_matrix;
};

#pragma once
#include <glm/glm.hpp>
#include <vector>

#include <OpenGL/VertexArray.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/IndexBuffer.hpp>
#include <OpenGL/ShaderProgram.hpp>

#include <Render/Camera.hpp>


class Block 
{
public:
    Block(const glm::vec3& position = {0.f, 0.f, 0.f}, const glm::vec3& scale = { 1.f, 1.f, 1.f }, const glm::vec3& rotation = { 0.f, 0.f, 0.f });
    ~Block();

    void draw(const ShaderProgram& shader, const Camera& camera) const;

    void set_scale(const glm::vec3& scale);
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);

private:
    VertexArray m_vao;
    VertexBuffer* m_vbo_point = nullptr;
    VertexBuffer* m_vbo_color = nullptr;
    IndexBuffer* m_ebo = nullptr;


    glm::mat4 m_pos_matrix;
    glm::mat4 m_scale_matrix;
    glm::mat4 m_rotate_matrix;
};

#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

class Camera
{
public:
    enum class ProjectionMode
    {
        Perspective,
        Orthographic
    };


    Camera(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 }, const ProjectionMode projection_mode = ProjectionMode::Perspective);

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
    void set_projection_mode(const ProjectionMode projection_mode);
    glm::mat4 get_view_matrix() const { return m_view_matrix; }
    glm::mat4 get_projection_matrix() const { return m_projection_matrix; }

    glm::vec3 get_position() const{ return m_position; }

    void set_rotate_delta(const glm::vec2& delta, float dt);

    void move_forward(const float delta, const float dt);
    void move_right(const float delta, const float dt);
    void move_up(const float delta, const float dt);
private:
    void update_view_matrix();
    void update_projection_matrix();

    static constexpr glm::vec3 s_world_forward{0.f, 0.f, -1.f};
    static constexpr glm::vec3 s_world_right{1.f, 0.f, 0.f};
    static constexpr glm::vec3 s_world_up{0.f, 1.f, 0.f};

    glm::vec3 m_position;
    glm::vec3 m_rotation; // X - Pitch, Y - Yaw, Z - Roll
    ProjectionMode m_projection_mode;
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;

    glm::vec3 m_direction;
    glm::vec3 m_up;
    glm::vec3 m_right;
};
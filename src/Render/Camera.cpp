#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <common/Log.hpp>

Camera::Camera(const glm::vec3& position,
    const glm::vec3& rotation,
    const ProjectionMode projection_mode)
    : m_position(position), 
      m_rotation(rotation), 
      m_projection_mode(projection_mode),
      m_direction(0.f, 0.f, 0.f),
      m_right(0.f, 0.f, 0.f),
      m_up(0.f, 0.f, 0.f)
{
    update_view_matrix();
    update_projection_matrix();
}

void Camera::update_view_matrix()
{
    float pitch_in_radians = glm::radians(m_rotation.x);
    float yaw_in_radians = glm::radians(m_rotation.y);
    float roll_in_radians = glm::radians(m_rotation.z);

    glm::mat3 rotate_matrix_x(1, 0, 0,
        0, cos(pitch_in_radians), sin(pitch_in_radians),
        0, -sin(pitch_in_radians), cos(pitch_in_radians));

    glm::mat3 rotate_matrix_y(cos(yaw_in_radians), 0, -sin(yaw_in_radians),
        0, 1, 0,
        sin(yaw_in_radians), 0, cos(yaw_in_radians));

    glm::mat3 rotate_matrix_z(cos(roll_in_radians), sin(roll_in_radians), 0,
        -sin(roll_in_radians), cos(roll_in_radians), 0,
        0, 0, 1);

    glm::mat4 translate_matrix(1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -m_position[0], -m_position[1], -m_position[2], 1);

    m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);

    glm::mat3 euler_matrix_rotate = rotate_matrix_x * rotate_matrix_z * rotate_matrix_y;

    m_direction = glm::normalize(glm::transpose(euler_matrix_rotate) * s_world_forward);
    m_right = glm::normalize(glm::transpose(euler_matrix_rotate) * s_world_right);
    m_up = glm::normalize(glm::transpose(euler_matrix_rotate) * s_world_up);
}

void Camera::update_projection_matrix()
{
    if (m_projection_mode == ProjectionMode::Perspective)
    {
        float r = 0.1f;
        float t = 0.1f;
        float f = 10000;
        float n = 0.1f;
        m_projection_matrix = glm::mat4(n / r, 0, 0, 0,
            0, n / t, 0, 0,
            0, 0, (-f - n) / (f - n), -1,
            0, 0, -2 * f * n / (f - n), 0);
    }
    else
    {
        float r = 2;
        float t = 2;
        float f = 10000;
        float n = 0.1f;
        m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
            0, 1 / t, 0, 0,
            0, 0, -2 / (f - n), 0,
            0, 0, (-f - n) / (f - n), 1);
    }
}

void Camera::set_position(const glm::vec3& position)
{
    m_position = position;
    update_view_matrix();
}

void Camera::set_rotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    update_view_matrix();
}

void Camera::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
{
    m_position = position;
    m_rotation = rotation;
    update_view_matrix();
}

void Camera::set_projection_mode(const ProjectionMode projection_mode)
{
    m_projection_mode = projection_mode;
    update_projection_matrix();
}

void Camera::set_rotate_delta(const glm::vec2& delta, float dt)
{
    m_rotation.x -= delta.y * dt * 22*6*3;
    m_rotation.y -= delta.x * dt * 22*6*3;
    if (m_rotation.x >= 90.f) m_rotation.x = 90.f;
    if (m_rotation.x <= -90.f) m_rotation.x = -90.f;
    update_view_matrix();
}

void Camera::move_forward(const float delta, const float dt)
{
    m_position += m_direction * delta * dt;
    update_view_matrix();
}

void Camera::move_right(const float delta, const float dt)
{
    m_position += m_right * delta * dt;
    update_view_matrix();
}

void Camera::move_up(const float delta, const float dt)
{
    m_position += m_up * delta * dt;
    update_view_matrix();
}

#include "Camera.hpp"

mylib::Camera::Camera(glm::vec3 pos, glm::vec3 up, float sensitivity, float zoom, float speed, float yaw, float pitch)
    : m_front{glm::vec3(0.0f, 0.0f, -1.0f)}, m_right{glm::vec3(1.0f, 0.0f, 0.0f)}
{
    m_pos = pos;
    m_up = up;
    m_sensitivity = sensitivity;
    m_zoom = zoom;
    m_speed = speed;
    m_yaw = yaw;
    m_pitch = pitch;
    m_allowMovement = true;
    m_allowCameraMovement = true;
    updateCameraVectors();
}

void mylib::Camera::updateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    m_front = glm::normalize(direction);
    m_right = glm::normalize(glm::cross(m_front, mylib::WORLD_UP));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void mylib::Camera::processKeyboard(mylib::CameraMovement direction, float deltaTime)
{
    if (!m_allowMovement)
        return;

    using namespace mylib;
    float velocity = m_speed * deltaTime;
    
    if (direction == CameraMovement::FORWARD)
        m_pos += velocity * m_front;
    if (direction == CameraMovement::BACKWARDS)
        m_pos -= velocity * m_front;
    if (direction == CameraMovement::RIGHT)
        m_pos += velocity * m_right;
    if (direction == CameraMovement::LEFT)
        m_pos -= velocity * m_right;
    if (direction == CameraMovement::UP)
        m_pos += velocity * WORLD_UP;
    if (direction == CameraMovement::DOWN)
        m_pos -= velocity * WORLD_UP;
}

// Takes in the mouse offset, the offset is the position - last position of the mouse.
// constrainPitch makes it so the camera can't break its neck
void mylib::Camera::processMouse(double xOffset, double yOffset, bool constrainPitch)
{
    if (!m_allowCameraMovement)
        return;
    
    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;

    m_yaw += (float)xOffset;
    m_pitch += (float)yOffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void mylib::Camera::processScroll(double yOffset)
{
    if (!m_allowCameraMovement)
        return;

    m_zoom -= (float)yOffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}
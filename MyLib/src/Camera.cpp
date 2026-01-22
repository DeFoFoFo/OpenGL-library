#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

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
    m_allow_movement = true;
    m_allow_looking = true;
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
    if (!m_allow_movement)
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
    if (!m_allow_looking)
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
    if (!m_allow_looking)
        return;

    m_zoom -= (float)yOffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

glm::mat4 mylib::Camera::getViewMatrix()
{
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

glm::vec3 mylib::Camera::getPos()
{
    return m_pos;
}
glm::vec3 mylib::Camera::getUp()
{
    return m_up;
}

glm::vec3 mylib::Camera::getFront()
{
    return m_front;
}
glm::vec3 mylib::Camera::getRight()
{
    return m_right;
}

float mylib::Camera::getSensitivity()
{
    return m_sensitivity;
}

float mylib::Camera::getZoom()
{
    return m_zoom;
}

float mylib::Camera::getSpeed()
{
    return m_speed;
}

float mylib::Camera::getYaw()
{
    return m_yaw;
}

float mylib::Camera::getPitch()
{
    return m_pitch;
}

bool mylib::Camera::isAllowedMoving()
{
    return m_allow_movement;
}

bool mylib::Camera::isAllowedLooking()
{
    return m_allow_looking;
}

void mylib::Camera::setPos(glm::vec3 pos)
{
    m_pos = pos;
}

void mylib::Camera::setDirection(glm::vec3 front, float yaw, float pitch)
{
    m_front = front;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

void mylib::Camera::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void mylib::Camera::setZoom(float zoom)
{
    m_zoom = zoom;
}

void mylib::Camera::setSpeed(float speed)
{
    m_speed = speed;
}

void mylib::Camera::setYaw(float yaw)
{
    m_yaw = yaw;
}

void mylib::Camera::setPitch(float pitch)
{
    m_pitch = pitch;
}

// If set to false, the camera can not move
void mylib::Camera::allowMovement(bool state)
{
    m_allow_movement = state;
}

// If set to false, the camera can not look around or zoom
void mylib::Camera::allowLooking(bool state)
{
    m_allow_looking = state;
}
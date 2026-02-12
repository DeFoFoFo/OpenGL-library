#pragma once

#include "glm/glm.hpp"

namespace mylib
{

enum class CameraMovement : uint32_t
{
    FORWARD,
    BACKWARDS,
    RIGHT,
    LEFT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

static const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera
{
public:
    explicit Camera(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                    float sensitivity = SENSITIVITY, float zoom = ZOOM, float speed = SPEED, float yaw = YAW, float pitch = PITCH);

    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouse(double xOffset, double yOffset, bool constrainPitch = true);
    void processScroll(double yOffset);

    // Getters
    glm::mat4 getViewMatrix();
    glm::vec3 getPos();
    glm::vec3 getUp();
    glm::vec3 getFront();
    glm::vec3 getRight();
    float getSensitivity();
    float getZoom();
    float getSpeed();
    float getYaw();
    float getPitch();
    bool isAllowedMoving();
    bool isAllowedLooking();

    // Setters
    void setPos(glm::vec3 pos);
    void setDirection(glm::vec3 front, float yaw, float pitch);
    void setSensitivity(float sensitivity);
    void setZoom(float zoom);
    void setSpeed(float speed);
    void setYaw(float yaw);
    void setPitch(float pitch);
    void allowMovement(bool state);
    void allowLooking(bool state);
    
private:
    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_right;
    float m_sensitivity;
    float m_zoom;
    float m_speed;
    float m_yaw, m_pitch;
    bool m_allowMovement;
    bool m_allowLooking;

    void updateCameraVectors();
};

} // namespace mylib
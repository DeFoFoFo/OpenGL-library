#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    constexpr inline glm::mat4 getViewMatrix() {return glm::lookAt(m_pos, m_pos + m_front, m_up);}
    constexpr inline glm::vec3 getPos() const {return m_pos;}
    constexpr inline glm::vec3 getUp() const {return m_up;}
    constexpr inline glm::vec3 getFront() const {return m_front;}
    constexpr inline glm::vec3 getRight() const {return m_right;}
    constexpr inline float getSensitivity() const {return m_sensitivity;}
    constexpr inline float getZoom() const {return m_zoom;}
    constexpr inline float getSpeed() const {return m_speed;}
    constexpr inline float getYaw() const {return m_yaw;}
    constexpr inline float getPitch() const {return m_pitch;}
    constexpr inline bool isAllowedMoving() const {return m_allowMovement;}
    constexpr inline bool isAllowedMovingCamera() const {return m_allowCameraMovement;}

    // Setters
    inline void setPos(glm::vec3 pos) {m_pos = pos;}
    inline void setDirection(glm::vec3 front, float yaw, float pitch) {m_front = front; m_yaw = yaw; m_pitch = pitch;}
    inline void setSensitivity(float sensitivity) {m_sensitivity = sensitivity;}
    inline void setZoom(float zoom) {m_zoom = zoom;}
    inline void setSpeed(float speed) {m_speed = speed;}
    inline void setYaw(float yaw) {m_yaw = yaw;}
    inline void setPitch(float pitch) {m_pitch = pitch;}
    inline void allowMovement(bool state) {m_allowMovement = state;}
    inline void allowMovingCamera(bool state) {m_allowCameraMovement = state;}
    
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
    bool m_allowCameraMovement;

    void updateCameraVectors();
};

} // namespace mylib
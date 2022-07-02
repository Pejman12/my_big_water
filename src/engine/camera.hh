#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include <vector>

// Default camera values
#define ALPHAX 0.0f
#define ALPHAY 225.0f
#define SPEED 0.5f
#define SENSITIVITY 0.3f

class Camera
{
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 world_up;
    glm::vec3 right;
    glm::vec3 up;

    // euler Angles
    float alphaY;
    float alphaX;
    // camera options
    float movementSpeed;
    float mouseSensitivity;

    // constructor with vectors
    Camera(glm::vec3 pos_ = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f),
           float alphaY_ = ALPHAY, float alphaX_ = ALPHAX, float movementSpeed_ = SPEED,
           float mouseSensitivity_ = SENSITIVITY);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float alphaY_, float alphaX_);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() noexcept;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(int direction) noexcept;
    
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch) noexcept;
    
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(int btn) noexcept;
    
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() noexcept;

};
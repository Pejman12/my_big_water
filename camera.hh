#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include <vector>


// Default camera values
const float ALPHAX         = -90.0f;
const float ALPHAY       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

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
    float zoom;

    // constructor with vectors
    Camera(glm::vec3 position, glm::vec3 up, float alphaY, float alphaX);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float alphaY, float alphaX);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(int direction, float deltaTime);
    
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
    
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);
    
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();

};
#include "camera.hh"


// constructor with vectors
Camera::Camera(glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f), float yaw_ = ALPHAY, float pitch_ = ALPHAX) {
    this->pos = position_;
    this->up = up_;
    this->alphaY = yaw_;
    this->alphaX = pitch_;
    updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_) {
    this->pos = glm::vec3(posX, posY, posZ);
    this->world_up = glm::vec3(upX, upY, upZ);
    this->alphaY = yaw_;
    this->alphaX = pitch_;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = this->movementSpeed * deltaTime;
    if (direction == GLUT_KEY_UP)
        this->pos += this->front * velocity;
    if (direction == GLUT_KEY_DOWN)
        this->pos -= this->front * velocity;
    if (direction == GLUT_KEY_LEFT)
        this->pos -= this->right * velocity;
    if (direction == GLUT_KEY_RIGHT)
        this->pos += this->right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->alphaY += xoffset;
    this->alphaX += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->alphaX > 89.0f)
            this->alphaX = 89.0f;
        if (this->alphaX < -89.0f)
            this->alphaX = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset) {
    this->zoom -= (float)yoffset;
    if (this->zoom < 1.0f)
        this->zoom = 1.0f;
    if (this->zoom > 45.0f)
        this->zoom = 45.0f; 
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->alphaY)) * cos(glm::radians(this->alphaX));
    front.y = sin(glm::radians(this->alphaX));
    front.z = sin(glm::radians(this->alphaY)) * cos(glm::radians(this->alphaX));
    this->front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
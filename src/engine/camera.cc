#include "camera.hh"


// constructor with vectors
Camera::Camera(glm::vec3 pos_, glm::vec3 up_, float alphaY_, float alphaX_,
               float movementSpeed_, float mouseSensitivity_)
    : pos(pos_)
    , world_up(up_)
    , alphaY(alphaY_)
    , alphaX(alphaX_)
    , movementSpeed(movementSpeed_)
    , mouseSensitivity(mouseSensitivity_) {
    updateCameraVectors();
}

// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
               float alphaY_, float alphaX_)
    : pos(posX, posY, posZ)
    , world_up(upX, upY, upZ)
    , alphaY(alphaY_)
    , alphaX(alphaX_)
    , movementSpeed(SPEED)
    , mouseSensitivity(SENSITIVITY) {
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() noexcept {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(int direction) noexcept {
    if (direction == GLUT_KEY_UP)
        this->pos += this->up * this->movementSpeed;
    if (direction == GLUT_KEY_DOWN)
        this->pos -= this->up * this->movementSpeed;
    if (direction == GLUT_KEY_LEFT)
        this->pos -= this->right * this->movementSpeed;
    if (direction == GLUT_KEY_RIGHT)
        this->pos += this->right * this->movementSpeed;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) noexcept {
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
void Camera::processMouseScroll(int btn) noexcept {
    if (btn == 3)
        this->pos += this->front * this->movementSpeed;
    else if (btn == 4)
        this->pos -= this->front * this->movementSpeed;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() noexcept {
    // calculate the new Front vector
    glm::vec3 front_;
    front_.x = cos(glm::radians(this->alphaY)) * cos(glm::radians(this->alphaX));
    front_.y = sin(glm::radians(this->alphaX));
    front_.z = sin(glm::radians(this->alphaY)) * cos(glm::radians(this->alphaX));
    this->front = glm::normalize(front_);
    // also re-calculate the Right and Up vector
    this->right = glm::normalize(glm::cross(this->front, this->world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

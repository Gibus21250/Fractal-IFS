//
// Created by nj123548 on 24/06/24.
//

#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

glm::mat4 Camera::getVPMatrix() const
{
    glm::mat4 view = glm::lookAt(this->position, this->lookAt, this->up);
    glm::mat4 projection = glm::perspective(glm::radians(this->fov), this->aspect, 0.0001f, 200.0f);

    return projection * view;
}

//----------------------------------------------------------\\

Camera::Camera()
    : position(0, 0, -1), lookAt(0, 0, 0), up(0, 1, 0), fov(90), aspect(16.0f/9) {}

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up)
    : position(pos), lookAt(lookAt), up(up), fov(80), aspect(16.0f/9) {}

Camera::Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, float fov, float aspect)
    : position(pos), lookAt(lookAt), up(up), fov(fov), aspect(aspect) {}

glm::vec3 &Camera::getPosition() {
    return this->position;
}

void Camera::setPosition(const glm::vec3 &nPosition) {
    Camera::position = nPosition;
}

const glm::vec3 &Camera::getLookAt() const {
    return lookAt;
}

void Camera::setLookAt(const glm::vec3 &nLookAt) {
    Camera::lookAt = nLookAt;
}

const glm::vec3 &Camera::getUp() const {
    return up;
}

void Camera::setUp(const glm::vec3 &nUp) {
    Camera::up = nUp;
}

float Camera::getFov() const {
    return fov;
}

void Camera::setFov(float fov) {
    Camera::fov = fov;
}

float Camera::getAspect() const {
    return aspect;
}

void Camera::setAspect(float aspect) {
    Camera::aspect = aspect;
}


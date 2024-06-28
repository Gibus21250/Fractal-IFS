//
// Created by nj123548 on 24/06/24.
//

#ifndef IFS_CAMERA_H
#define IFS_CAMERA_H

#include <glm/glm.hpp>

class Camera {

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up);
    Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up, float fov, float aspect);

    glm::vec3 &getPosition();
    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getLookAt() const;
    void setLookAt(const glm::vec3 &lookAt);

    const glm::vec3 &getUp() const;
    void setUp(const glm::vec3 &up);

    float getFov() const;
    void setFov(float fov);

    float getAspect() const;
    void setAspect(float aspect);

    //---------------------------------------------------------------\\

    glm::mat4 getVPMatrix() const;

private:
    glm::vec3 position, lookAt, up{0, -1, 0};
    float fov;
    float aspect;

};


#endif //IFS_CAMERA_H

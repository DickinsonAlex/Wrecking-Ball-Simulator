// Camera.cpp
#include "Camera.h"

Camera::Camera() {}

Camera::Camera(const physx::PxVec3& position, const physx::PxVec3& direction)
    : eye(position), dir(direction.getNormalized()) {
}

void Camera::SetPosition(const physx::PxVec3& position)
{
    eye = position;
}

void Camera::SetDirection(const physx::PxVec3& direction)
{
    dir = direction.getNormalized();
}

physx::PxVec3 Camera::GetEye() const
{
    return eye;
}

physx::PxVec3 Camera::GetDir() const
{
    return dir;
}
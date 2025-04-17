// Camera.h
#pragma once
#include <PxPhysicsAPI.h>

class Camera
{
public:
    Camera();
    Camera(const physx::PxVec3& position, const physx::PxVec3& direction);

    void SetPosition(const physx::PxVec3& position);
    void SetDirection(const physx::PxVec3& direction);

    physx::PxVec3 GetEye() const;
    physx::PxVec3 GetDir() const;

private:
    physx::PxVec3 eye;
    physx::PxVec3 dir;
};
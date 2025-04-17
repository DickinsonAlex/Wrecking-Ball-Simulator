// Crane.h
#pragma once
#include <vector>
#include <PxPhysicsAPI.h>
#include "Actors.h"
#include "Joints.h"
#include "Camera.h"
#include "InputManager.h"

class Crane
{
public:
    Crane(const physx::PxTransform& pose);
    ~Crane();

    void Update(physx::PxReal deltaTime, InputManager* inputManager, Camera* camera);
    std::vector<Actor*> GetActors();

private:
    DynamicActor* base;
    DynamicActor* ball;
    DistanceJoint* joint;
};
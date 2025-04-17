// Joints.h
#pragma once
#include <PxPhysicsAPI.h>
#include "Actors.h"

class Joint
{
public:
    Joint(Actor* a0, Actor* a1) : actor0(a0), actor1(a1) {}
    virtual ~Joint() {}

protected:
    Actor* actor0;
    Actor* actor1;
    physx::PxJoint* joint = nullptr;
};

class DistanceJoint : public Joint
{
public:
    DistanceJoint(Actor* a0, const physx::PxTransform& local0, Actor* a1, const physx::PxTransform& local1);
    void SetStiffness(float value);
    void SetDamping(float value);
};
// Joints.cpp
#include "Joints.h"
#include "Engine.h"

DistanceJoint::DistanceJoint(Actor* a0, const physx::PxTransform& local0, Actor* a1, const physx::PxTransform& local1)
    : Joint(a0, a1)
{
    joint = physx::PxDistanceJointCreate(*Engine::GetPhysics(),
        a0->GetActor(), local0,
        a1->GetActor(), local1);

    joint->setConstraintFlag(physx::PxConstraintFlag::eVISUALIZATION, true);
    ((physx::PxDistanceJoint*)joint)->setDistanceJointFlag(physx::PxDistanceJointFlag::eSPRING_ENABLED, true);

    SetStiffness(5.f);
    SetDamping(1.f);
}

void DistanceJoint::SetStiffness(float value)
{
    ((physx::PxDistanceJoint*)joint)->setStiffness(value);
}

void DistanceJoint::SetDamping(float value)
{
    ((physx::PxDistanceJoint*)joint)->setDamping(value);
}
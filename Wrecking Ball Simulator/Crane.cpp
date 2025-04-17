// Crane.cpp
#include <PxPhysicsAPI.h>
#include "Crane.h"
#include "Engine.h"

Crane::Crane(const physx::PxTransform& pose)
{
    base = new DynamicActor(pose);
    base->CreateShape(physx::PxBoxGeometry(2.f, 1.f, 3.f), 20.f);
    base->SetColour(physx::PxVec3(1.f, 0.f, 0.f));
    base->SetName("CraneBase");

    ball = new DynamicActor(physx::PxTransform(pose.p + physx::PxVec3(0.f, -5.f, 0.f)));
    ball->CreateShape(physx::PxSphereGeometry(1.f), 5.f);
    ball->SetColour(physx::PxVec3(0.1f, 0.1f, 0.1f));
    ball->SetName("Ball");

    joint = new DistanceJoint(base, physx::PxTransform(physx::PxVec3(0.f, -1.f, 0.f)), ball, physx::PxTransform(physx::PxVec3(0.f, 1.f, 0.f)));
    joint->SetStiffness(5.f);
    joint->SetDamping(1.f);
}

Crane::~Crane()
{
    delete base;
    delete ball;
    delete joint;
}

void Crane::Update(physx::PxReal deltaTime, InputManager* inputManager, Camera* camera)
{
    physx::PxVec3 force(0.f);

    if (inputManager->IsKeyDown('w')) force.z -= 1.f;
    if (inputManager->IsKeyDown('s')) force.z += 1.f;
    if (inputManager->IsKeyDown('a')) force.x -= 1.f;
    if (inputManager->IsKeyDown('d')) force.x += 1.f;

    if (force.magnitude() > 0)
    {
        force = force.getNormalized() * 500.f;
        ((physx::PxRigidDynamic*)base->GetActor())->addForce(force);
    }
}

std::vector<Actor*> Crane::GetActors()
{
    return { base, ball };
}
// Scene.cpp
#include <PxPhysicsAPI.h>
#include <stdexcept>
#include "Scene.h"
#include "Engine.h"
#include "Crane.h"
#include "Plane.h"

void Scene::Init(Camera* cam)
{
    camera = cam;

    physx::PxSceneDesc sceneDesc(Engine::GetPhysics()->getTolerancesScale());
    sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;

    if (!sceneDesc.cpuDispatcher)
    {
        physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }

    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    pxScene = Engine::GetPhysics()->createScene(sceneDesc);

    if (!pxScene)
        throw std::runtime_error("Could not initialize Scene");

    pxScene->setGravity(physx::PxVec3(0.f, -9.81f, 0.f));

    plane = new Plane(physx::PxVec3(0.f, 1.f, 0.f));
    AddActor(plane);

    crane = new Crane(physx::PxTransform(physx::PxVec3(0.f, 0.f, 0.f)));
    AddActors(crane->GetActors());
}

void Scene::CustomUpdate(physx::PxReal deltaTime, InputManager* inputManager)
{
    if (crane)
        crane->Update(deltaTime, inputManager, camera);
}

void Scene::AddActor(Actor* actor)
{
    if (actor && actor->GetActor())
        pxScene->addActor(*actor->GetActor());
}

void Scene::AddActors(const std::vector<Actor*>& actors)
{
    for (Actor* actor : actors)
        AddActor(actor);
}


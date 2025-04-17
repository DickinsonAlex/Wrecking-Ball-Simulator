// SceneBase.cpp
#include "SceneBase.h"

void SceneBase::Init(Camera* camera)
{
    this->camera = camera;
}

std::vector<physx::PxActor*> SceneBase::GetPxActors()
{
    std::vector<physx::PxActor*> pxActors;
    if (pxScene)
    {
        physx::PxU32 actorCount = pxScene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC);
        if (actorCount)
        {
            pxActors.resize(actorCount);
            pxScene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC | physx::PxActorTypeFlag::eRIGID_DYNAMIC, pxActors.data(), actorCount);
        }
    }
    return pxActors;
}
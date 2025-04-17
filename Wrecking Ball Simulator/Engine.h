// Engine.h
#pragma once
#include "Camera.h"
#include "Scene.h"
#include "InputManager.h"
#include <PxPhysicsAPI.h>

namespace Engine
{
    void Init(const char* title, int width, int height);

	void PxInit();
	void PxRelease();

    void Start();

    physx::PxPhysics* GetPhysics();
    physx::PxCooking* GetCooking();
    Scene* GetScene();
    float GetUptime();

    physx::PxMaterial* GetMaterial(physx::PxU32 index = 0);
    physx::PxMaterial* CreateMaterial(physx::PxReal staticFriction = 0.5f, physx::PxReal dynamicFriction = 0.5f, physx::PxReal restitution = 0.1f);

    physx::PxConvexMeshGeometry CreateConvexMeshGeometry(std::vector<physx::PxVec3>& verts, float x = 0.f, float y = 0.f, float z = 0.f, physx::PxVec3 rotation = physx::PxVec3(0.f, 0.f, 0.f));

    extern physx::PxVec3 DefaultColour;
}
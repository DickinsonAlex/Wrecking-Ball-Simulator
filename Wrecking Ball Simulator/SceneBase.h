// SceneBase.h
#pragma once
#include "Camera.h"
#include "HUD.h"
#include "InputManager.h"
#include "Actors.h"
#include <vector>
#include <PxPhysicsAPI.h>

class SceneBase
{
public:
    virtual void Init(Camera* camera);
    virtual void CustomInit() {}
    virtual void CustomUpdate(physx::PxReal deltaTime, InputManager* inputManager) {}

    virtual void KeyDown(unsigned char key) {}
    virtual void MouseMove(physx::PxVec2 delta, bool focused) {}

    physx::PxScene* GetScene() { return pxScene; }
    virtual std::vector<physx::PxActor*> GetPxActors();

protected:
    physx::PxScene* pxScene = nullptr;
    Camera* camera = nullptr;
    HUD hud;
};
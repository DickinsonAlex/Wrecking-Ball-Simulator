#pragma once
#include <PxPhysicsAPI.h>
#include <stdexcept>
#include "SceneBase.h"
#include "Crane.h"
#include "Plane.h"

class Scene : public SceneBase
{
public:
    void Init(Camera* camera) override;
    void CustomUpdate(physx::PxReal deltaTime, InputManager* inputManager) override;

protected:
    void AddActor(Actor* actor);
    void AddActors(const std::vector<Actor*>& actors);

private:
    Crane* crane = nullptr;
    Plane* plane = nullptr;
};

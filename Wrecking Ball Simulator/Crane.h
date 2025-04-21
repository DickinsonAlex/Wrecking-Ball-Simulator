// Crane.h
#pragma once

#include "Collider.h"
#include "Joints.h"
#include "InputManager.h"
#include "Camera.h"
#include <vector>

#ifndef Crane_h
#define Crane_h

class CraneBase : public StaticActor
{
public:
    CraneBase(const PxTransform& pose = PxTransform(PxIdentity),
        float baseSize = 2.5f,
        float beamThickness = .15f,
        float sectionLength = 2.f,
        float height = 80.f);
};

class CraneArm : public Actor
{
public:
    CraneArm(const PxTransform& pose = PxTransform(PxIdentity),
        float beamThickness = .15f,
        float sectionLength = 2.f);
    ~CraneArm();

    // Expose actors for rendering/physics
    std::vector<Actor*> getActors() const;

private:
    StaticActor* lowerArm;
    DynamicActor* upperArm;
    RevoluteJoint* elbowJoint;
};

class CraneHook : public StaticActor
{
public:
    CraneHook(const PxTransform& pose = PxTransform(PxIdentity),
        float baseSize = 2.5f,
        float beamThickness = .15f,
        float length = 50.f,
        float hookSize = 2.f);

    float getMinExtension() const { return minExtension; }
    float getMaxExtension() const { return maxExtension; }
    float getExtensionSpeed() const { return extensionSpeed; }

private:
    float minExtension, maxExtension;
    float extensionSpeed = 0.1f;
};

class ChainLink : public DynamicActor
{
public:
    ChainLink(const PxTransform& pose = PxTransform(PxIdentity));

    static float getSize() { return 1.f; }
};

class Chain
{
public:
    Chain(Actor* start, PxVec3 startOffset, Actor* end, PxVec3 endOffset);
    ~Chain();

    float getLength() const { return length; }
    std::vector<Actor*> getActors() const;

private:
    float length;
    std::vector<ChainLink*> links;
    std::vector<RevoluteJoint*> joints;
};

class Wheel : public StaticActor
{
public:
    Wheel(const PxTransform& pose);
};

class WreckingBall : public Collider
{
public:
    WreckingBall(const PxTransform& pose = PxTransform(PxIdentity),
        float radius = 1.f,
        float density = 1.f);

    void CustomUpdate(float deltaTime) override {}
    void OnCollision(Collider* other) override;
};

class Crane : public Actor
{
public:
    Crane(const PxTransform& pose = PxTransform(PxIdentity),
        float baseSize = 2.5f,
        float chassisHeight = .15f,
        float sectionLength = 2.f,
        float height = 80.f,
        float length = 50.f);
    ~Crane();

    void Update(PxReal deltaTime, InputManager* inputManager, Camera* camera) override;
    std::vector<Actor*> getActors() override;

private:
    float baseSize, beamThickness, sectionLength, height, length;

    CraneBase* base;
    CraneArm* arm;
    CraneHook* hook;
    Chain* chain;
    WreckingBall* ball;
    std::vector<Wheel*> wheels;

    float rotationSpeed = 0.005f;
};

#endif // Crane_h

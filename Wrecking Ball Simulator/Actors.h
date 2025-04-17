// Actors.h
#pragma once
#include <PxPhysicsAPI.h>
#include <vector>
#include <string>

class Actor
{
public:
    virtual ~Actor() {}
    virtual physx::PxRigidActor* GetActor() { return actor; }
    void SetColour(physx::PxVec3 colour, physx::PxU32 shapeIndex = -1);
    void SetName(const std::string& name);
    std::string GetName();
    physx::PxShape* GetShape(physx::PxU32 shapeIndex = 0);

protected:
    physx::PxRigidActor* actor = nullptr;
    std::vector<physx::PxVec3> colours;
    std::string name;
};

class DynamicActor : public Actor
{
public:
    DynamicActor(const physx::PxTransform& pose);
    virtual ~DynamicActor();
    void CreateShape(const physx::PxGeometry& geometry, physx::PxReal density = 1.f);
    void SetKinematic(bool value);
};

class StaticActor : public Actor
{
public:
    StaticActor(const physx::PxTransform& pose);
    virtual ~StaticActor();
    void CreateShape(const physx::PxGeometry& geometry, physx::PxReal density = 0.f);
};

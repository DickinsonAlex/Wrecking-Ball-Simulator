// Actors.cpp
#include "Actors.h"
#include "Engine.h"
#include "UserData.h"

void Actor::SetColour(physx::PxVec3 colour, physx::PxU32 shapeIndex)
{
    if (shapeIndex == -1)
        for (unsigned int i = 0; i < colours.size(); i++)
            colours[i] = colour;
    else if (shapeIndex < colours.size())
        colours[shapeIndex] = colour;
}

void Actor::SetName(const std::string& n)
{
    name = n;
    if (actor) actor->setName(n.c_str());
}

std::string Actor::GetName() { return name; }

physx::PxShape* Actor::GetShape(physx::PxU32 shapeIndex)
{
    std::vector<physx::PxShape*> shapes(((physx::PxRigidActor*)actor)->getNbShapes());
    if (shapeIndex < ((physx::PxRigidActor*)actor)->getShapes((physx::PxShape**)&shapes.front(), (physx::PxU32)shapes.size()))
        return shapes[shapeIndex];
    return nullptr;
}

DynamicActor::DynamicActor(const physx::PxTransform& pose)
{
    actor = (physx::PxRigidActor*)Engine::GetPhysics()->createRigidDynamic(pose);
    SetName("DynamicActor");
}

DynamicActor::~DynamicActor() {}

void DynamicActor::CreateShape(const physx::PxGeometry& geometry, physx::PxReal density)
{
    physx::PxShape* shape = ((physx::PxRigidDynamic*)actor)->createShape(geometry, *Engine::GetMaterial());
    physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidDynamic*)actor, density);
    colours.push_back(Engine::DefaultColour);
    shape->userData = new UserData();
    ((UserData*)shape->userData)->colour = &colours.back();
}

void DynamicActor::SetKinematic(bool value)
{
    ((physx::PxRigidDynamic*)actor)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, value);
}

StaticActor::StaticActor(const physx::PxTransform& pose)
{
    actor = (physx::PxRigidActor*)Engine::GetPhysics()->createRigidStatic(pose);
    SetName("StaticActor");
}

StaticActor::~StaticActor() {}

void StaticActor::CreateShape(const physx::PxGeometry& geometry, physx::PxReal density)
{
    physx::PxShape* shape = ((physx::PxRigidStatic*)actor)->createShape(geometry, *Engine::GetMaterial());
    colours.push_back(Engine::DefaultColour);
    shape->userData = new UserData();
    ((UserData*)shape->userData)->colour = &colours.back();
}
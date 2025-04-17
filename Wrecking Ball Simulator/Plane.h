#pragma once
#include "Actors.h"

class Plane : public StaticActor
{
public:
    Plane(const physx::PxVec3& normal);
};

#include "Plane.h"
#include <PxPhysicsAPI.h>
#include "Engine.h"

Plane::Plane(const physx::PxVec3& normal)
    : StaticActor(physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.f, 0.f, 1.f))))
{
    CreateShape(physx::PxPlaneGeometry());
    SetName("GroundPlane");
    SetColour(physx::PxVec3(0.6f, 0.6f, 0.6f));
}

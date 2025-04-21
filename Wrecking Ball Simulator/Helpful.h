#pragma once
#include <PxPhysicsAPI.h>   

using namespace physx;

namespace Helpful
{
	static PxVec3 RGBtoScalar(float r, float g, float b) {
		return PxVec3(r / 255.f, g / 255.f, b / 255.f);
	}
}

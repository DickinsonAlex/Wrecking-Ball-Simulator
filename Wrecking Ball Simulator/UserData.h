#pragma once
#include <PxPhysicsAPI.h>

#ifndef USERDATA_H
#define USERDATA_H

using namespace physx;

class UserData
{
public:
	PxVec3* color;
	PxClothMeshDesc* clothMeshDesc;

	UserData(PxVec3* colour = 0, PxClothMeshDesc* clothMeshDesc = 0)
		: color(colour), clothMeshDesc(clothMeshDesc) {
	}
};

#endif // !USERDATA_H
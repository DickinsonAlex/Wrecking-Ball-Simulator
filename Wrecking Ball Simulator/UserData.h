#pragma once
#include <PxPhysicsAPI.h>

#ifndef UserData_h
#define UserData_h

using namespace physx;

class UserData
{
public:
	PxVec3* colour;
	PxClothMeshDesc* clothMeshDesc;

	UserData(PxVec3* colour = 0, PxClothMeshDesc* clothMeshDesc = 0)
		: colour(colour), clothMeshDesc(clothMeshDesc) {
	}
};

#endif // !UserData_h
#pragma once

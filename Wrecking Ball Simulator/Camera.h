#pragma once
#include "foundation/PxTransform.h"
#include "foundation/PxQuat.h"
#include "Actor.h" // Include Actor class

#ifndef Camera_h
#define Camera_h

using namespace physx;

class Camera
{
public:
	Camera(); // Constructor
	~Camera(); // Destructor

	// Getters
	PxVec3 getPosition() const;
	PxVec3 getDirection() const;

	// Update
	void update(float deltaTime, Actor* target);
private:
	PxVec3 position, eyeInit; // Camera position
	PxVec3 orientation, dirInit; // Camera rotation
};

#endif // CAMERA_H
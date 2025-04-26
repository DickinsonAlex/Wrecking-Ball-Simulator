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

	// Target
	Actor* targetActor; // Pointer to the target actor

	// Getters
	PxVec3 getPosition() const;
	PxVec3 getDirection() const;
	Actor* getTargetActor() { return targetActor; };

	// Update
	void update(float deltaTime, Actor* target);
private:
	PxVec3 position, eyeInit; // Camera position
	PxVec3 orientation, dirInit; // Camera rotation
};

#endif // CAMERA_H
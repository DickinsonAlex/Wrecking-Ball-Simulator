#pragma once
#include "foundation/PxTransform.h"
#include "foundation/PxMat33.h"
#include "foundation/PxQuat.h"
#include "Actor.h" // Include Actor class

#ifndef Camera_h
#define Camera_h

using namespace physx;

class Camera
{
public:
	Camera(Actor* target); // Constructor
	~Camera(); // Destructor

	// Target
	Actor* targetActor; // Pointer to the target actor

	// Getters
	PxVec3 getPosition() const;
	PxVec3 getDirection() const;
	Actor* getTargetActor() { return targetActor; };
	
	// Setters
	void setTargetActor(Actor* actor);

	// Update
	void update(float deltaTime);
private:
	PxVec3 position, eyeInit; // Camera position
	PxVec3 orientation, dirInit; // Camera rotation
};

#endif // CAMERA_H
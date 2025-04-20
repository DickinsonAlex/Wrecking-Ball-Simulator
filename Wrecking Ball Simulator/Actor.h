#pragma once
#include <PxPhysicsAPI.h>
#include <string>

#ifndef ACTOR_H
#define ACTOR_H

using namespace std;
using namespace physx;

class Actor
{
protected:
	int id; // Actor ID
	PxRigidActor* pxActor; // Pointer to the actor
	string name; // Actor name

public:
	Actor(); // Constructor
	~Actor(); // Destructor

	// Getters
	PxRigidActor* getPxActor() { return pxActor; }; // Get the PxRigidActor pointer
	string getName() { return name;  }; // Get the actor name
	int getId() { return id; }; // Get the actor ID
	PxVec3 getPosition() const; // Get the actor position
	PxQuat getRotation() const; // Get the actor rotation

	// Setters
	void setName(const string& name); // Set the actor name
	void setId(int id); // Set the actor ID
	void setPosition(const PxVec3& position); // Set the actor position
	void setRotation(const PxQuat& rotation); // Set the actor rotation
};

#endif // ACTOR_H


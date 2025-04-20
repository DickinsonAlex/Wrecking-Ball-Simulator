#include "Actor.h"
#include <iostream>

// Constructor
Actor::Actor() : id(0), pxActor(nullptr), name("") {}

// Destructor
Actor::~Actor() {
    if (pxActor) {
        pxActor->release(); // Release the PxRigidActor if it exists
        pxActor = nullptr;
    }
}

// Get the actor position
PxVec3 Actor::getPosition() const {
    if (pxActor) {
        return pxActor->getGlobalPose().p; // Retrieve position from PxTransform
    }
    return PxVec3(0.0f, 0.0f, 0.0f); // Default position if pxActor is null
}

// Get the actor rotation
PxQuat Actor::getRotation() const {
    if (pxActor) {
        return pxActor->getGlobalPose().q; // Retrieve rotation from PxTransform
    }
    return PxQuat(PxIDENTITY()); // Default rotation if pxActor is null
}

// Set the actor name
void Actor::setName(const string& newName) {
    name = newName;
}

// Set the actor ID
void Actor::setId(int newId) {
    id = newId;
}

// Set the actor position
void Actor::setPosition(const PxVec3& position) {
    if (pxActor) {
        PxTransform transform = pxActor->getGlobalPose();
        transform.p = position;
        pxActor->setGlobalPose(transform);
    }
}

// Set the actor rotation
void Actor::setRotation(const PxQuat& rotation) {
    if (pxActor) {
        PxTransform transform = pxActor->getGlobalPose();
        transform.q = rotation;
        pxActor->setGlobalPose(transform);
    }
}
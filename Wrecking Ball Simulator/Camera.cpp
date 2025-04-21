#include "Camera.h"
#include "Actor.h" // Include Actor class

Camera::Camera(Actor* target)
{
	targetActor = target; // Initialize target actor
	position = PxVec3(0, 0, 0); // Default camera position
	orientation = PxVec3(0, 0, 0); // Default camera orientation
}

Camera::~Camera()
{
}

PxVec3 Camera::getPosition() const
{
    return position;
}

PxVec3 Camera::getOrientation() const
{
    return orientation;
}

void Camera::setTargetActor(Actor* actor)
{
    targetActor = actor;
}

void Camera::update(float deltaTime)
{
    if (targetActor)
    {
        // Update camera position and rotation based on the target actor
        position = targetActor->getPosition();
        orientation = targetActor->getOrientation();
    }
}
#include "Camera.h"
#include "Actor.h" // Include Actor class

Camera::Camera(Actor* target)
{
	targetActor = target; // Initialize target actor
	position = PxVec3(0, 0, 0); // Default camera position
	rotation = PxQuat(0, 0, 0, 1); // Default camera rotation
}

Camera::~Camera()
{
}

PxVec3 Camera::getPosition() const
{
    return position;
}

PxQuat Camera::getRotation() const
{
    return rotation;
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
        rotation = targetActor->getRotation();
    }
}
#include "Camera.h"
#include "Actor.h" // Include Actor class

Camera::Camera()
{
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

PxVec3 Camera::getDirection() const
{
    return orientation;
}

void Camera::update(float deltaTime, Actor* target)
{
    if (target)
    {
        // Update camera position to be behind the top of the crane, so relative to the angle its facing
		position = target->getPosition() + PxVec3(0, 35, -70); // Adjust the offset as needed
		// Update camera orientation to look at the target
		orientation = target->getPosition() - position; // Look at the target
		orientation.normalize(); // Normalize the direction vector
    }
}
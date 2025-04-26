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
        // Update the camera's position to follow the target actor, maintaining a fixed offset.
        position = target->getPosition() + PxVec3(0, 35, -70); // Adjust the offset as needed.

        // Update the camera's orientation to look directly at the target actor.
        orientation = target->getPosition() - position; // Calculate the direction vector.
        orientation.normalize(); // Normalize the direction vector for consistent behavior.
    }
}
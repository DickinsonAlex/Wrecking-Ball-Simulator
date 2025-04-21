#include "Level.h"
#include "Helpful.h"
#include <GL/glut.h>

void Level::spawnFeatures()
{
    // Implementation for spawning features in the scene
    plane = new Plane(PxVec3(.0f, 1.0f, .0f));
	plane->setColour(Helpful::RGBtoScalar(91.f, 163.f, 90.f));
	plane->setName("Ground");
    addActor(plane);
}

void Level::updateFeatures(float deltaTime, InputManager* inputManager)
{
    // Implementation for updating features in the scene
    return;
}
#include "Level.h"
#include <GL/glut.h>

void Level::spawnFeatures()
{
    // Implementation for spawning features in the scene
    plane = new Plane(PxVec3(.0f, 1.0f, .0f));
    plane->setColour(PxVec3(194.f / 255.f, 178.f / 255.f, 128.f / 255.f));
    addActor(plane);
}

void Level::updateFeatures(float deltaTime, InputManager* inputManager)
{
    // Implementation for updating features in the scene
    return;
}
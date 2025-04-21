#include "Level.h"
#include "Helpful.h"
#include <GL/glut.h>

void Level::spawnFeatures()
{
    // Implementation for spawning features in the scene
    ground = new Plane(PxVec3(.0f, 1.0f, .0f));
	ground->setColour(Helpful::RGBtoScalar(91.f, 163.f, 90.f));
	ground->setName("Ground");
    addActor(ground);

	house = new House(PxTransform(PxVec3(0.f, 0.f, 0.f)));
	house->setName("House");
	addActors(house->GetActors());

    wrecker = new Wrecker(PxTransform(PxVec3(0.f, 0.f, 0.f)));
    wrecker->setName("Wrecking Ball Machine");
    addActors(wrecker->GetActors());


}

void Level::updateFeatures(float deltaTime, InputManager* inputManager)
{
    // Implementation for updating features in the scene
    return;
}
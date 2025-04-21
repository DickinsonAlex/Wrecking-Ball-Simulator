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

	// Create the house and add it to the scene
	structure_1 = new Structure(PxTransform(PxVec3(10.f, 10.f, 80.f)), 3, 4, 4); // Pose, floors, width, depth)
	addActors(structure_1->getActors());

	printf("Level features spawned successfully.\n");
}

void Level::updateFeatures(float deltaTime, InputManager* inputManager)
{
    // Implementation for updating features in the scene like the camera, crane, and destruction
    return;
}

vector<Actor*> Level::getActors()
{
    vector<Actor*> actors;

    actors.push_back(ground);
    //for (Actor* actor : wrecker->GetActors())
     //   actors.push_back(actor);
    for (Actor* actor : structure_1->getActors())
        actors.push_back(actor);

    return actors;
}
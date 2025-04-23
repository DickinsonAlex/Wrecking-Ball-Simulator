#include "Level.h"
#include "Helpful.h"
#include <GL/glut.h>

void Level::spawnFeatures()
{
	// Create the crane
	crane = new Crane(PxTransform(PxVec3(-10.f, 10.f, 0.f)), 5.f, 1.f);
	addActors(crane->getActors());

    // Implementation for spawning features in the scene
    ground = new Plane(PxVec3(.0f, 1.0f, .0f));
	ground->setColour(Helpful::RGBtoScalar(91.f, 163.f, 90.f));
	ground->setName("Ground");
    addActor(ground);


	// Create multiple buildings and add it to the scene
	//building_1 = new Structure(PxTransform(PxVec3(10.f, 10.f, 80.f)), 2, 3, 3); // Pose, floors, width, depth)

	printf("Level features spawned successfully.\n");
}

void Level::updateFeatures(float deltaTime, InputManager* inputManager, Camera* camera)
{
	crane->Update(deltaTime, inputManager, camera);
}

vector<Actor*> Level::getActors()
{
    vector<Actor*> actors;

    actors.push_back(ground);
	actors.push_back(crane);

    //for (Actor* actor : building_1->getActors())
    //    actors.push_back(actor);

    return actors;
}
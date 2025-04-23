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
	building_1 = new Structure(PxTransform(PxVec3(10.f, 10.f, 80.f)), 2, 3, 3); // Pose, floors, width, depth)
	building_2 = new Structure(PxTransform(PxVec3(-10.f, 10.f, 80.f)), 2, 3, 3);
	building_3 = new Structure(PxTransform(PxVec3(10.f, 10.f, -80.f)), 2, 3, 3);
	building_4 = new Structure(PxTransform(PxVec3(-10.f, 10.f, -80.f)), 2, 3, 3);
	building_5 = new Structure(PxTransform(PxVec3(-20.f, 10.f, 35.f)), 2, 3, 3);

	addActors(building_1->getActors());
	addActors(building_2->getActors());
	addActors(building_3->getActors());
	addActors(building_4->getActors());
	addActors(building_5->getActors());

	printf("Level features spawned successfully.\n");
}

void Level::updateFeatures(float deltaTime, InputManager* inputManager, Camera* camera)
{
	if (inputManager->isKeyPressed(static_cast<unsigned char>('w')))
		crane->Move(PxVec2(0.0f, 1.0f)); // Move forward
	if (inputManager->isKeyPressed(static_cast<unsigned char>('s')))
		crane->Move(PxVec2(0.0f, -1.0f)); // Move backward
	if (inputManager->isKeyPressed(static_cast<unsigned char>('a')))
		crane->Move(PxVec2(-1.0f, 0.0f)); // Move left
	if (inputManager->isKeyPressed(static_cast<unsigned char>('d')))
		crane->Move(PxVec2(1.0f, 0.0f)); // Move right
}

vector<Actor*> Level::getActors()
{
    vector<Actor*> actors;

    actors.push_back(ground);
    for (Actor* actor : building_1->getActors())
        actors.push_back(actor);

	for (Actor* actor : building_2->getActors())
		actors.push_back(actor);

	for (Actor* actor : building_3->getActors())
		actors.push_back(actor);

	for (Actor* actor : building_4->getActors())
		actors.push_back(actor);

	for (Actor* actor : building_5->getActors())
		actors.push_back(actor);

    return actors;
}
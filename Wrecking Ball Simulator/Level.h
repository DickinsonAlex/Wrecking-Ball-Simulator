#pragma once
#include "Scene.h"
#include "Primatives.h"
#include "Structure.h"
#include "Crane.h"

#ifndef LEVEL_H
#define LEVEL_H

class Level : public Scene
{
private:
	Plane* ground; // Plane object for the level
	wallStructure* wall; // Wall structure object for the level
	Crane* crane; // Crane object for the level
protected:
    void spawnFeatures(); // Spawns the initial features of the level, such as walls and the crane.
    void updateFeatures(float deltaTime, InputManager* inputManager, Camera* camera); // Updates the level's features based on user input and time.
public:
    vector<Actor*> getActors(); // Retrieves all actors present in the level.
    Actor* getTarget() override { return crane->getTop(); } // Returns the top of the crane as the target actor.
};

#endif // LEVEL_H

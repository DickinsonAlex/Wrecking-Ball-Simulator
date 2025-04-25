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
	void spawnFeatures(); // Spawn features in the scene
	void updateFeatures(float deltaTime, InputManager* inputManager, Camera* camera); // Update features in the scene
public:
	vector<Actor*> getActors(); // Get all sub-actors in the scene
	Actor* getTarget() { return crane; } // Get the target actor (crane)
};

#endif // LEVEL_H

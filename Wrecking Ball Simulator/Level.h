#pragma once
#include "Scene.h"
#include "Primatives.h"

#ifndef LEVEL_H
#define LEVEL_H

class Level : public Scene
{
private:
	Plane* ground; // Plane object for the level
protected:
	void spawnFeatures(); // Spawn features in the scene
	void updateFeatures(float deltaTime, InputManager* inputManager); // Update features in the scene
};

#endif // LEVEL_H

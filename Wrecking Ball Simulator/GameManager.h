#pragma once
#include "Scene.h" // Include Scene class
#include "InputManager.h" // Include InputManager class
#include "Actor.h" // Include Actor class
#include "Renderer.h" // Include Renderer class

#ifndef GameManager_h
#define GameManager_h

class GameManager
{
private: 
	void Start(); // Initialize the game engine
	void Init();
	void Update();

	// From PhysX Tutorials
	void PxInit();
	void PxShutdown();
	PxMaterial* GetMaterial(PxU32 index = 0);
	PxMaterial* CreateMaterial(PxReal staticFriction = .0f, PxReal dynamicFriction = .0f, PxReal coefficientRestitution = .0f);


	Scene* scene; // Pointer to the scene
	PxPhysics* physics; // Pointer to the physics engine
	PxCooking* cooking; // Pointer to the cooking engine
	float Uptime; // Uptime of the game

public:
	// Getters
	Scene* getScene() { return scene; };
	PxPhysics* getPhysics() { return physics; };
	PxCooking* getCooking() { return cooking; };
	float GetUptime() { return Uptime; }; // Get the uptime of the game

	// Setters
	void setScene(Scene* scene);
};

#endif // GAMEMANAGER_H
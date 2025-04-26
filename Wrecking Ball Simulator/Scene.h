#include <vector> // Include vector for list of actors  
#include <iostream> // Include iostream for console output
#include "Camera.h" // Include Camera class
#include "Actor.h" // Include Actor class
#include "InputManager.h" // Include InputManager class
#include <PxPhysicsAPI.h> // Include PhysX API for PxRigidActor

using namespace std;

#ifndef SCENE_H
#define SCENE_H

class Scene
{
protected:
	PxScene* pxScene;
	bool paused;
	Camera* camera; // Ensure camera is declared
	InputManager* inputManager; // Ensure inputManager is declared
	vector<Actor*> actors; // Ensure actors vector is declared

	// Virtual Functions for spawning and updating features
	virtual void spawnFeatures() = 0; // Pure virtual function for spawning features
	virtual void updateFeatures(float deltaTime, InputManager* inputManager, Camera* camera) = 0; // Pure virtual function for updating features
public:
	void Init(Camera* cam, InputManager* iM); // Initialize the scene  
	void Update(float deltaTime, InputManager* inputManager, Camera* camera); // Update the scene

	// Getters  
	Camera* getCamera(); // Get the camera  
	vector<Actor*> getActors(); // Get the list of actors  
	vector<PxActor*> getPxActors(); // Get the list of PxActors
	PxScene* getScene() { return pxScene; }
	virtual Actor* getTarget() = 0;

	// Setters  
	void setCamera(Camera* camera); // Set the camera  
	void addActor(Actor* actor); // Add an actor to the list  
	void addActors(vector<Actor*> actorList); // Add a list of actors to the scene
	void removeActor(Actor* actor); // Remove an actor from the list  
	void setPaused(bool value); // Set the paused state
};

#endif // SCENE_H

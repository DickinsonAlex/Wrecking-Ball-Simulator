#pragma once
#include <PxPhysicsAPI.h>

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

using namespace physx;

class InputManager
{
public:
	InputManager(); // Constructor
	~InputManager(); // Destructor

	void Init();
	void Update();

	// Getters
	bool isKeyPressed(int key) const;
	PxVec2 getMousePosition() const;

	// Setters
	void setKeyPressed(int key);
	void setKeyReleased(int key);
	void setMouse(int button, int state, int x, int y); // Set mouse position
};

#endif // INPUTMANAGER_H


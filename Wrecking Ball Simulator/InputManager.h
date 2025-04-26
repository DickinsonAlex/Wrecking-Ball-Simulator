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

	void Init(int x, int y);
	void Update();

	// Getters
	bool isKeyPressed(unsigned char key) const;

	// Setters
	void reshapeWindow(int width, int height); // Update the viewport and projection matrix
	void setKeyPressed(unsigned char key);
	void setKeyReleased(unsigned char key);
};

#endif // INPUTMANAGER_H


#pragma once
#include <PxPhysicsAPI.h>

#ifndef InputManager_h
#define InputManager_h

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
	bool isKeyDown(int key) const;
	PxVec2 getMousePosition() const;
};

#endif // INPUTMANAGER_H


// Crane.h
#pragma once

#include "Collider.h"
#include "Joints.h"
#include "InputManager.h"
#include "Camera.h"
#include <vector>

#ifndef Crane_h
#define Crane_h

using namespace physx;
using namespace std;


// --- Wheel Class ---
class Wheel : public DynamicActor
{
public:
	Wheel(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~Wheel();
private:
	float size;
};

// --- CraneBottom Class ---
class CraneBottom : public DynamicActor
{
public:
	CraneBottom(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~CraneBottom();
	vector<Actor*> getActors();
private:
	float size;
	Wheel* wheels[4];
};

// --- CraneTop Class ---
class CraneTop : public DynamicActor
{
public:
	CraneTop(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f, CraneBottom* bottom = nullptr);
	~CraneTop();
	vector<Actor*> getActors();
private:
	float size;
	float length;
	CraneBottom* bottom;
};

// --- Crane Class ---
class Crane : public DynamicActor
{
public:
	Crane(const PxTransform& pose = PxTransform(PxIdentity), float Size = 1.0f, float Length = 1.0f);
	~Crane();
	void Update(float deltaTime, InputManager* inputManager, Camera* camera);
	vector<Actor*> getActors();
	void Move(PxVec2 movementOffset);
	void Rotate(Actor* target, float direction);
private:
	float Size, Length;
	CraneBottom* bottom;
	CraneTop* top;

	float rotationSpeed = 0.005f;
};

#endif // Crane_h
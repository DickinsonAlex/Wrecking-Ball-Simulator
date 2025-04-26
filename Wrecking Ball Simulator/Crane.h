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

// --- CraneArm Class ---
class CraneArm : public DynamicActor
{
public:
	CraneArm(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneArm();
private:
	float size;
	float length;
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
	std::vector<Actor*> actors; // Store the actors in a vector
};

// --- Crane Class ---
class Crane : public DynamicActor
{
public:
	Crane(const PxTransform& pose = PxTransform(PxIdentity), float Size = 1.0f, float Length = 1.0f);
	~Crane();
	Actor* getTop();
	void Update(float deltaTime, InputManager* inputManager);
	vector<Actor*> getActors();
	void Move(PxVec2 movementOffset);
	void Rotate(Actor* target, float direction);
private:
	float Size, Length;
	CraneBottom* bottom;
	CraneTop* top;
	//CraneChain* chain;
	//CraneBall* ball;

	float rotationSpeed = 0.005f;
};

#endif // Crane_h
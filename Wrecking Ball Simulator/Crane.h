// Crane.h
#pragma once

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

// --- WreckingBall Class ---
class WreckingBall : public DynamicActor
{
public:
	WreckingBall(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~WreckingBall();
private:
	float size;
};

// --- CraneChain Class ---
class CraneChain : public DynamicActor
{
public:
	CraneChain(const PxTransform& pose = PxTransform(PxIdentity), float chainSegmentLength = 1.f);
	~CraneChain();
};


// --- CraneArm Class ---
class CraneArmBottom : public DynamicActor
{
public:
	CraneArmBottom(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneArmBottom();
private:
	float size;
	float length;
};

class CraneArmTop : public DynamicActor
{
public:
	CraneArmTop(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneArmTop();
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

	float rotationSpeed = 0.005f;
};

#endif // Crane_h
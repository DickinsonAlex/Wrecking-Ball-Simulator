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

// --- CraneTop Class ---
class CraneTop : public StaticActor
{
public:
	CraneTop(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneTop();
	vector<Actor*> getActors();
private:
	float size;
	float length;
};

// --- Wheel Class ---
class Wheel : public StaticActor
{
public:
	Wheel(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~Wheel();
private:
	float size;
};

// --- CraneBottom Class ---
class CraneBottom : public StaticActor
{
public:
	CraneBottom(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~CraneBottom();
	vector<Actor*> getActors();
private:
	float size;
	Wheel* wheels[4];
};

// --- Crane Class ---
class Crane : public DynamicActor
{
public:
	Crane(const PxTransform& pose = PxTransform(PxIdentity), float Size = 1.0f, float Length = 1.0f);
	~Crane();
	vector<Actor*> getActors();

	void Move(PxVec2 movementOffset);
private:
	float Size, Length;

	CraneBottom* bottom;
	CraneTop* top;
	DynamicActor* thePoint;

	float rotationSpeed = 0.005f;
};

#endif // Crane_h

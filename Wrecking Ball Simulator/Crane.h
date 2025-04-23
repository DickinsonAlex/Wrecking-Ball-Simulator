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
class Wheel : public StaticActor
{
public:
	Wheel(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~Wheel(); 
private:
	float size;
};

// --- Wheels Class ---
class Wheels : public StaticActor
{
public:
	Wheels(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~Wheels();
	std::vector<Actor*> getActors();
private:
	Wheel* wheels[4];
};

// --- CraneBase Class ---
class CraneBase : public StaticActor
{
public:
	CraneBase(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~CraneBase();
private:
	float size;
};

// --- CraneCore Class ---
class CraneCore : public StaticActor
{
public:
	CraneCore(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~CraneCore();
private:
	float size;
};

// --- CraneLowerArm Class ---
class CraneLowerArm : public StaticActor
{
public:
	CraneLowerArm(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneLowerArm() { delete this; } // Destructor
private:
	float size;
	float length;
};

// --- CraneUpperArm Class ---
class CraneUpperArm : public StaticActor
{
public:
	CraneUpperArm(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
	~CraneUpperArm() { delete this; } // Destructor
private:
	float size;
	float length;
};

// --- CraneArm Class ---
class CraneArm : public StaticActor
{
public:
	CraneArm(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f, float length = 1.0f);
    ~CraneArm() {
        // Clean up the arm components
        delete lowerArm;
        delete upperArm;
    }
	std::vector<Actor*> getActors();
private:
	CraneLowerArm* lowerArm; // The lower arm of the crane connected to the core
	CraneUpperArm* upperArm; // The upper arm of the crane connected to the lower arm
	float size;
	float length; // Declare 'length' as a member of the class
};

// --- CraneHook Class ---
class CraneHook : public StaticActor
{
public:
	CraneHook(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~CraneHook();
private:
	float size;
};

// --- Chain Class ---
class Chain : public StaticActor
{
public:
	Chain(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~Chain();
private:
	float size;
};

// --- WreckingBall Class ---
class WreckingBall : public StaticActor
{
public:
    WreckingBall(const PxTransform& pose = PxTransform(PxIdentity), float size = 1.0f);
	~WreckingBall();

	void onColision(Actor* other);
private:
	float size;
};

// --- Crane Class ---
class Crane : public Actor
{
public:
    Crane(const PxTransform& pose = PxTransform(PxIdentity), float Size = 1.0f, float Length = 1.0f);
    ~Crane();

    void Update(PxReal deltaTime, InputManager* inputManager, Camera* camera);
    std::vector<Actor*> getActors();

private:
    float Size, Length;

	Wheels* wheels; // The wheels of the crane connected to the base
	CraneBase* base; // The base of the crane connected to the wheels and the core
	CraneCore* core; // The core of the crane connected by a joint to the base, and the arm
	CraneArm* arm; // The arm of the crane connected to the core
	CraneHook* hook; // The hook of the crane connected to the arm
	Chain* chain; // The chain of the crane connected to the hook and the wrecking ball
	WreckingBall* ball; // The wrecking ball connected to the chain

    float rotationSpeed = 0.005f;
};

#endif // Crane_h

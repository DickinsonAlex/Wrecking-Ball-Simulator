#pragma once
// No changes needed in the header file for this modification.
#include "Collider.h"
#include "Joints.h"
#include "InputManager.h"
#include "Camera.h"

#ifndef Crane_h
#define Crane_h

class CraneShaft : public StaticActor
{
public:
	CraneShaft(const PxTransform& pose = PxTransform(PxIdentity), float baseSize = 2.5f, float beamThickness = .15f, float sectionLength = 2.f, float height = 80.f);
};

class CraneArm : public StaticActor
{
public:
	CraneArm(const PxTransform& pose = PxTransform(PxIdentity), float baseSize = 2.5f, float beamThickness = .15f, float sectionLength = 2.f, float length = 50.f);
};

class CraneHook : public StaticActor
{
public:
	CraneHook(const PxTransform& pose = PxTransform(PxIdentity), float baseSize = 2.5f, float beamThickness = .15f, float length = 50.f, float hookSize = 2.f);

	float getMinExtension() const { return minExtension; }
	float getMaxExtension() const { return maxExtension; }

	float getExtensionSpeed() const { return extensionSpeed; }
private:
	float minExtension, maxExtension;
	float extensionSpeed = 0.1f;
};

class ChainLink : public DynamicActor
{
public:
	ChainLink(const PxTransform& pose = PxTransform(PxIdentity));

	static float getSize() { return 1.f; }
};

class Chain
{
public:
	Chain(Actor* start, PxVec3 startOffset, Actor* end, PxVec3 endOffset);
	~Chain();

	float getLength() const { return length; }
	vector<Actor*> getActors();
private:
	float length;
	vector<ChainLink*> links;
	vector<RevoluteJoint*> joints;
};

class WreckingBall : public Collider
{
public:
	WreckingBall(const PxTransform& pose = PxTransform(PxIdentity), float radius = 1.f, float density = 1.f);

	void CustomUpdate(float deltaTime) {}

	void OnCollision(Collider* other);
};

class Crane : public Actor
{
public:
	Crane(const PxTransform& pose = PxTransform(PxIdentity), float baseSize = 2.5f, float beamThickness = .15f, float sectionLength = 2.f, float height = 80.f, float length = 50.f);
	~Crane();

	void Update(PxReal deltaTime, InputManager* inputManager, Camera* camera);

	vector<Actor*> getActors();

private:
	float baseSize, beamThickness, sectionLength, height, length;

	CraneShaft* shaft;
	CraneArm* arm;
	CraneHook* hook;
	Chain* chain;
	WreckingBall* ball;

	float rotationSpeed = 0.005f;
};

#endif // !Crane_h
#pragma once
#include "Actor.h"
#include <PxPhysicsAPI.h>

#ifndef JOINTS_H
#define JOINTS_H

using namespace physx;

// From PhysX Tutorials
class Joint
{
protected:
	PxJoint* joint;
public:
	Joint(Actor* actor0, Actor* actor1) : joint(0) {}
	~Joint() { if (joint) joint->release(); }

	PxJoint* GetJoint() { return joint; }
};

// From PhysX Tutorials
class DistanceJoint : public Joint
{
public:
	DistanceJoint(Actor* actor0, const PxTransform& localTransform0, Actor* actor1, const PxTransform& localTransform1);

	void SetStiffness(PxReal value) { ((PxDistanceJoint*)joint)->setStiffness(value); }
	PxReal GetStiffness() const { return ((PxDistanceJoint*)joint)->getStiffness(); }

	void SetDamping(PxReal value) { ((PxDistanceJoint*)joint)->setDamping(value); }
	PxReal GetDamping() const { return ((PxDistanceJoint*)joint)->getDamping(); }
};

// From PhysX Tutorials
class RevoluteJoint : public Joint
{
public:
	RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

	void SetLimits(PxReal lower, PxReal upper);

	void SetDriveVelocity(PxReal value);
	PxReal GetDriveVelocity() const { return ((PxRevoluteJoint*)joint)->getDriveVelocity(); }
};

// From PhysX Tutorials
class FixedJoint : public Joint
{
public:
	FixedJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);
};

// Custom
class D6Joint : public Joint
{
public:
	D6Joint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

	void SetMotion(PxD6Axis::Enum axis, PxD6Motion::Enum motion);
	void SetTwistLimit(PxReal lower, PxReal upper);
	void SetSwingLimit(PxReal yLimit, PxReal zLimit);
};


#endif // !JOINTS_H
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

	void setStiffness(PxReal value) { ((PxDistanceJoint*)joint)->setStiffness(value); }
	PxReal GetStiffness() const { return ((PxDistanceJoint*)joint)->getStiffness(); }

	void setDamping(PxReal value) { ((PxDistanceJoint*)joint)->setDamping(value); }
	PxReal GetDamping() const { return ((PxDistanceJoint*)joint)->getDamping(); }
};

// From PhysX Tutorials
class RevoluteJoint : public Joint
{
public:
	RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

	void setLimits(PxReal lower, PxReal upper);
	void setLimit(PxJointAngularLimitPair limit) { ((PxRevoluteJoint*)joint)->setLimit(limit); }
	void setProjectionLinearTolerance(PxReal tolerance) { ((PxRevoluteJoint*)joint)->setProjectionLinearTolerance(tolerance); }
	void setProjectionAngularTolerance(PxReal tolerance) { ((PxRevoluteJoint*)joint)->setProjectionAngularTolerance(tolerance); }

	void setDriveVelocity(PxReal value);
	PxReal GetDriveVelocity() const { return ((PxRevoluteJoint*)joint)->getDriveVelocity(); }
};

// From PhysX Tutorials
class FixedJoint : public Joint
{
public:
	FixedJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);
};

#endif // !JOINTS_H
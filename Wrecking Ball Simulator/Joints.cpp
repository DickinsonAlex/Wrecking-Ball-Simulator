#include "Joints.h"
#include "PhysicsEngine.h"

DistanceJoint::DistanceJoint(Actor* actor0, const PxTransform& localTransform0, Actor* actor1, const PxTransform& localTransform1)
	: Joint(actor0, actor1)
{
	PxRigidActor* pxActors[2] = { actor0->getPxActor(), actor1->getPxActor() };

	joint = PxDistanceJointCreate(*PhysicsEngine::getPhysics(), pxActors[0], localTransform0, pxActors[1], localTransform1);
	joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);

	setDamping(1.f);
	setStiffness(1.f);
}

RevoluteJoint::RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	: Joint(actor0, actor1)
{
	PxRigidActor* pxActors[2] = { actor0->getPxActor(), actor1->getPxActor() };

	joint = PxRevoluteJointCreate(*PhysicsEngine::getPhysics(), pxActors[0], localFrame0, pxActors[1], localFrame1);
	joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
}

void RevoluteJoint::setLimits(PxReal lower, PxReal upper)
{
	((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
	((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
}

void RevoluteJoint::setDriveVelocity(PxReal value)
{
	PxRigidDynamic* actor0 = nullptr;
	PxRigidDynamic* actor1 = nullptr;

	((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor0, (PxRigidActor*&)actor1);

	// Ensure actors are valid, dynamic, and in a scene before accessing them
	if (actor0 && actor0->getConcreteType() == PxConcreteType::eRIGID_DYNAMIC && actor0->getScene())
	{
		if (actor0->isSleeping())
			actor0->wakeUp();
	}

	if (actor1 && actor1->getConcreteType() == PxConcreteType::eRIGID_DYNAMIC && actor1->getScene())
	{
		if (actor1->isSleeping())
			actor1->wakeUp();
	}

	// Enable drive and set velocity if the joint exists
	if (joint)
	{
		((PxRevoluteJoint*)joint)->setDriveVelocity(value);
		((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
	}
}

FixedJoint::FixedJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	: Joint(actor0, actor1)
{
	PxRigidActor* pxActors[2] = { actor0->getPxActor(), actor1->getPxActor() };

	joint = PxFixedJointCreate(*PhysicsEngine::getPhysics(), pxActors[0], localFrame0, pxActors[1], localFrame1);
	joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
}

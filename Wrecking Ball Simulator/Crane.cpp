#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"

#include <PxPhysicsAPI.h>
#include <vector> // Include for std::vector

Crane::Crane(const PxTransform& pose, float baseSize, float chassisHeight, float sectionLength, float height, float length)
	: baseSize(baseSize), beamThickness(chassisHeight), sectionLength(sectionLength), height(height), length(length)
{
	// Create a realistic base with wheels
	base = new CraneBase(
		PxTransform(PxVec3(pose.p.x, pose.p.y, pose.p.z), pose.q),
		baseSize,
		chassisHeight,
		sectionLength,
		height
	);

	// Add wheels to the base
    wheels.push_back(new Wheel(PxTransform(PxVec3(pose.p.x - baseSize / 2, pose.p.y, pose.p.z - baseSize / 2), pose.q)));  
    wheels.push_back(new Wheel(PxTransform(PxVec3(pose.p.x + baseSize / 2, pose.p.y, pose.p.z - baseSize / 2), pose.q)));  
    wheels.push_back(new Wheel(PxTransform(PxVec3(pose.p.x - baseSize / 2, pose.p.y, pose.p.z + baseSize / 2), pose.q)));  
    wheels.push_back(new Wheel(PxTransform(PxVec3(pose.p.x + baseSize / 2, pose.p.y, pose.p.z + baseSize / 2), pose.q)));

	// Adjust arm and hook positions to match the realistic crane
	arm = new CraneArm(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y, pose.p.z), pose.q),
		baseSize,
		chassisHeight,
		sectionLength,
		length
	);

	float hookSize = 1.5f; // Adjust hook size
	hook = new CraneHook(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y, pose.p.z), pose.q),
		baseSize,
		chassisHeight,
		length,
		hookSize
	);

	float wreckingBallRadius = 1.5f; // Adjust wrecking ball size
	ball = new WreckingBall(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y - wreckingBallRadius - (hookSize * 0.6f), pose.p.z), pose.q),
		wreckingBallRadius,
		1.f
	);

	// Replace chain with small box links
	chain = new Chain((Actor*)hook, PxVec3(0.f, -(hookSize * 0.6f), hook->getMinExtension()), (Actor*)ball, PxVec3(0.f, wreckingBallRadius, 0.f));
}

Crane::~Crane()
{
	delete base;
	delete arm;
	delete hook;
	delete chain;
	delete ball;
	delete wheel;
}

void Crane::Update(PxReal deltaTime, InputManager* inputManager, Camera* camera)
{
	return;
}

vector<Actor*> Crane::getActors()
{
	vector<Actor*> actors;
	actors.push_back((Actor*)base);
	actors.push_back((Actor*)arm);
	actors.push_back((Actor*)hook);
	actors.push_back((Actor*)ball);

	for (Actor* link : chain->getActors())
		actors.push_back(link);

	// Add wheels to the list of actors
	for (Actor* wheel : wheels)
		actors.push_back((Actor*)wheel);

	return actors;
}

CraneBase::CraneBase(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float height)
	: StaticActor(pose)
{
	vector<PxVec3> baseVertices = {
		PxVec3(-baseSize / 2, 0.f, -baseSize / 2),
		PxVec3(-baseSize / 2, 0.f, baseSize / 2),
		PxVec3(baseSize / 2, 0.f, baseSize / 2),
		PxVec3(baseSize / 2, 0.f, -baseSize / 2),

		PxVec3(-baseSize / 2, beamThickness, -baseSize / 2),
		PxVec3(-baseSize / 2, beamThickness, baseSize / 2),
		PxVec3(baseSize / 2, beamThickness, baseSize / 2),
		PxVec3(baseSize / 2, beamThickness, -baseSize / 2)
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(baseVertices), 1.0f);

	// Set the color to yellow
	setColour(PxVec3(1.f, 1.f, 0.f));
}

// Remove beam logic from CraneArm
CraneArm::CraneArm(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float length)
	: StaticActor(pose)
{
	vector<PxVec3> armVertices = {
		PxVec3(-beamThickness / 2, 0.f, -baseSize / 2),
		PxVec3(-beamThickness / 2, 0.f, baseSize / 2),
		PxVec3(beamThickness / 2, 0.f, baseSize / 2),
		PxVec3(beamThickness / 2, 0.f, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, baseSize / 2),
		PxVec3(beamThickness / 2, beamThickness, -baseSize / 2),
		PxVec3(-beamThickness / 2, beamThickness, -baseSize / 2),
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(armVertices), 1.0f);

	// Set the color to green for debugging
	setColour(PxVec3(0.f, 1.f, 0.f));
}

// Remove beam logic from CraneHook
CraneHook::CraneHook(const PxTransform& pose, float baseSize, float beamThickness, float length, float hookSize)
	: StaticActor(PxTransform(pose.p + PxVec3(0.f, 0.f, 1.f + baseSize / 2 + hookSize / 2), pose.q))
{
	maxExtension = length - 2.f - baseSize / 2 - hookSize;
	minExtension = 1.f + baseSize / 2 + hookSize / 2;

	createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
		{
			PxVec3(-hookSize / 2, 0.f, -hookSize / 2),
			PxVec3(-hookSize / 2, 0.f, hookSize / 2),
			PxVec3(hookSize / 2, 0.f, hookSize / 2),
			PxVec3(hookSize / 2, 0.f, -hookSize / 2),

			PxVec3(-hookSize / 2, -hookSize, -hookSize / 2),
			PxVec3(-hookSize / 2, -hookSize, hookSize / 2),
			PxVec3(hookSize / 2, -hookSize, hookSize / 2),
			PxVec3(hookSize / 2, -hookSize, -hookSize / 2)
		}
		)), 1.0f);

	// Set the color to orange for debugging
	setColour(PxVec3(1.f, 0.5f, 0.f));
}

// Make the grey boxes thinner
ChainLink::ChainLink(const PxTransform& pose)
	: DynamicActor(pose)
{
	// Reduce the dimensions of the box geometry
	createShape(PxBoxGeometry(PxVec3(0.3f, 0.3f, 0.3f)), 1.0f); // Thinner box dimensions

	// Set the color to dark gray for debugging
	setColour(PxVec3(0.3f, 0.3f, 0.3f));
	setName("ChainLink");
}

Chain::Chain(Actor* start, PxVec3 startOffset, Actor* end, PxVec3 endOffset)
{
	float distance = ((end->getPxActor()->getGlobalPose().p + endOffset) - (start->getPxActor()->getGlobalPose().p + startOffset)).magnitude();
	this->length = floor(distance / ChainLink::getSize()) * ChainLink::getSize();

	PxTransform startGlobalPose = PxTransform(
		PxVec3(
			start->getPxActor()->getGlobalPose().p.x + startOffset.x,
			start->getPxActor()->getGlobalPose().p.y + startOffset.y,
			start->getPxActor()->getGlobalPose().p.z + startOffset.z
		),
		start->getPxActor()->getGlobalPose().q
	);

	Actor* previous = start;
	PxTransform previousOffset = PxTransform(startOffset);

	for (int i = 0; i < floor(distance / ChainLink::getSize()); i++)
	{
		ChainLink* link = 
			new ChainLink(PxTransform(
				PxVec3(
					startGlobalPose.p.x,
					startGlobalPose.p.y - i * ChainLink::getSize(),
					startGlobalPose.p.z
				),
				startGlobalPose.q
			));
		links.push_back(link);

		RevoluteJoint* joint = new RevoluteJoint(previous, previousOffset, link, PxTransform(PxVec3(0.f, ChainLink::getSize() / 2, 0.f)));
		joint->setLimits(-PxPi / 8, PxPi / 8);
		joint->setDriveVelocity(2.f);

		joints.push_back(joint);

		previous = link;
		previousOffset = PxTransform(PxVec3(0.f, -ChainLink::getSize() / 2, 0.f));
	}

	RevoluteJoint* joint = new RevoluteJoint(previous, previousOffset, end, PxTransform(endOffset));
	joint->setLimits(-PxPi / 8, PxPi / 8);
	joint->setDriveVelocity(2.f);
}

Chain::~Chain()
{
	for (Actor* link : links)
		delete link;
	for (RevoluteJoint* joint : joints)
		delete joint;
}

vector<Actor*> Chain::getActors()
{
	vector<Actor*> actors;
	for (Actor* link : links)
		actors.push_back(link);

	return actors;
}

WreckingBall::WreckingBall(const PxTransform& pose, float radius, float density)
	: Collider(pose)
{
	createShape(PxSphereGeometry(radius), density);
	
	// Set the color to red for debugging
	setColour(PxVec3(1.f, 0.f, 0.f));

	setName("WreckingBall");
}

void WreckingBall::OnCollision(Collider* other)
{
	// Wait for the building to settle before checking for collisions
	if (PhysicsEngine::getUptime() < 3.f) return;

	if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(other))
	{
		PxVec3 velocity = ((PxRigidDynamic*)getPxActor())->getLinearVelocity();
		PxVec3 colliderVelocity = ((PxRigidDynamic*)material->getPxActor())->getLinearVelocity();

		PxVec3 impulse = PxVec3(
			((PxReal)velocity.x - (PxReal)colliderVelocity.x) * ((PxRigidDynamic*)getPxActor())->getMass() * 10.f,
			((PxReal)velocity.y - (PxReal)colliderVelocity.y) * ((PxRigidDynamic*)getPxActor())->getMass() * 10.f,
			((PxReal)velocity.z - (PxReal)colliderVelocity.z) * ((PxRigidDynamic*)getPxActor())->getMass() * 10.f
		);

		float impulseMagnitude = impulse.magnitude();

		if (impulseMagnitude > material->getBreakForce())
			material->Break();
	}
}

Wheel::Wheel(const PxTransform& pose)
	: StaticActor(pose)
{
	// Add a cylinder shape to represent the wheel
	createShape(PxBoxGeometry(PxVec3(1.f, 1.5f, 1.5f)), 1.0f);
	
	// Set the color to black for debugging
	setColour(PxVec3(0.f, 0.f, 0.f));
}
#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"

#include <PxPhysicsAPI.h>

Crane::Crane(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float height, float length)
	: baseSize(baseSize), beamThickness(beamThickness), sectionLength(sectionLength), height(height), length(length)
{
	// Adjust dimensions for a smaller crane
	baseSize = 5.0f; // Smaller base size
	height = 10.0f;  // Reduced height
	length = 8.0f;   // Shorter arm length

	// Create a solid box bottom for the crane
	shaft = new CraneShaft(
		PxTransform(PxVec3(pose.p.x, pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		sectionLength,
		height
	);

	// Adjust arm and hook positions to match the smaller crane
	arm = new CraneArm(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		sectionLength,
		length
	);

	float hookSize = 1.5f; // Adjust hook size
	hook = new CraneHook(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		length,
		hookSize
	);

	float wreckingBallRadius = 1.5f; // Adjust wrecking ball size
	ball = new WreckingBall(
		PxTransform(PxVec3(pose.p.x, height + pose.p.y - wreckingBallRadius - (hookSize * 0.6f), pose.p.z), pose.q),
		wreckingBallRadius,
		1.f
	);

	chain = new Chain((Actor*)hook, PxVec3(0.f, -(hookSize * 0.6f), hook->getMinExtension()), (Actor*)ball, PxVec3(0.f, wreckingBallRadius, 0.f));
}

Crane::~Crane()
{
	delete shaft;
	delete arm;
	delete hook;
	delete chain;
	delete ball;
}

void Crane::Update(PxReal deltaTime, InputManager* inputManager, Camera* camera)
{
	return;
}

vector<Actor*> Crane::getActors()
{
	vector<Actor*> actors;
	actors.push_back((Actor*)shaft);
	actors.push_back((Actor*)arm);
	actors.push_back((Actor*)hook);
	actors.push_back((Actor*)ball);

	for (Actor* link : chain->getActors())
		actors.push_back(link);

	return actors;
}

CraneShaft::CraneShaft(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float height)
	: StaticActor(pose)
{
	// Replace beam-based structure with a solid box bottom
	vector<PxVec3> boxVertices = {
		PxVec3(-baseSize / 2, 0.f, -baseSize / 2),
		PxVec3(-baseSize / 2, 0.f, baseSize / 2),
		PxVec3(baseSize / 2, 0.f, baseSize / 2),
		PxVec3(baseSize / 2, 0.f, -baseSize / 2),

		PxVec3(-baseSize / 2, height, -baseSize / 2),
		PxVec3(-baseSize / 2, height, baseSize / 2),
		PxVec3(baseSize / 2, height, baseSize / 2),
		PxVec3(baseSize / 2, height, -baseSize / 2)
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(boxVertices), 1.0f);

	// Set the color to yellow
	setColour(PxVec3(1.f, 1.f, 0.f));
}

CraneArm::CraneArm(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float length)
	: StaticActor(pose)
{
	// Adjust arm dimensions for the smaller crane
	vector<PxVec3> armVertices = {
		PxVec3(-beamThickness / 2, 0.f, 0.f),
		PxVec3(-beamThickness / 2, 0.f, length),
		PxVec3(beamThickness / 2, 0.f, length),
		PxVec3(beamThickness / 2, 0.f, 0.f),

		PxVec3(-beamThickness / 2, beamThickness, 0.f),
		PxVec3(-beamThickness / 2, beamThickness, length),
		PxVec3(beamThickness / 2, beamThickness, length),
		PxVec3(beamThickness / 2, beamThickness, 0.f)
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(armVertices), 1.0f);

	// Set the color to yellow
	setColour(PxVec3(1.f, 1.f, 0.f));
}

CraneHook::CraneHook(const PxTransform& pose, float baseSize, float beamThickness, float length, float hookSize)
	: StaticActor(PxTransform(pose.p + PxVec3(0.f, 0.f, 1.f + baseSize / 2 + hookSize / 2), pose.q))
{
	maxExtension = length - 2.f - baseSize / 2 - hookSize;
	minExtension = 1.f + baseSize / 2 + hookSize / 2;

	createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
		{
			PxVec3(-beamThickness / 2, 0.f, -hookSize / 2),
			PxVec3(-beamThickness / 2, 0.f, hookSize / 2),
			PxVec3(beamThickness / 2, 0.f, hookSize / 2),
			PxVec3(beamThickness / 2, 0.f, -hookSize / 2),

			PxVec3(-beamThickness / 2, -beamThickness, -hookSize / 2),
			PxVec3(-beamThickness / 2, -beamThickness, hookSize / 2),
			PxVec3(beamThickness / 2, -beamThickness, hookSize / 2),
			PxVec3(beamThickness / 2, -beamThickness, -hookSize / 2),

			PxVec3(-beamThickness / 2, -hookSize * .6f, -beamThickness),
			PxVec3(-beamThickness / 2, -hookSize * .6f, beamThickness),
			PxVec3(beamThickness / 2, -hookSize * .6f, beamThickness),
			PxVec3(beamThickness / 2, -hookSize * .6f, -beamThickness),
		}
		), 0.f, 0.f, minExtension), 1.0f);

	setColour(PxVec3(.8f, .8f, 0.f));
}

ChainLink::ChainLink(const PxTransform& pose)
	: DynamicActor(pose)
{
	createShape(PxSphereGeometry(ChainLink::getSize() / 2), 1.0f);
	setColour(PxVec3(0.1f, 0.1f, 0.1f));
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
		ChainLink* link = new ChainLink(PxTransform(
			PxVec3(
				startGlobalPose.p.x,
				startGlobalPose.p.y - i * ChainLink::getSize(),
				startGlobalPose.p.z
			),
			startGlobalPose.q
		));
		links.push_back(link);

		for (int j = 0; j < 4; j++)
		{
			RevoluteJoint* joint = new RevoluteJoint(previous, previousOffset, link, PxTransform(PxVec3(0.f, ChainLink::getSize() / 2, 0.f)));
			joint->setLimits(-PxPi / 8, PxPi / 8);
			joint->setDriveVelocity(2.f);

			joints.push_back(joint);
		}

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
	setColour(PxVec3(0.05f, 0.05f, 0.05f));

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
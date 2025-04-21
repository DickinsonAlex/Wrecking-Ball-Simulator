#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"

#include <PxPhysicsAPI.h>

Crane::Crane(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float height, float length)
	: baseSize(baseSize), beamThickness(beamThickness), sectionLength(sectionLength), height(height), length(length)
{
	shaft = new CraneShaft(
		PxTransform(PxVec3(pose.p.x, pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		sectionLength,
		height
	);
	arm = new CraneArm(
		PxTransform(PxVec3(pose.p.x, (floor(height / sectionLength) * sectionLength) + beamThickness + pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		sectionLength,
		length
	);

	float hookSize = 2.f;

	hook = new CraneHook(
		PxTransform(PxVec3(pose.p.x, (floor(height / sectionLength) * sectionLength) + beamThickness + pose.p.y, pose.p.z), pose.q),
		baseSize,
		beamThickness,
		floor(length / sectionLength) * sectionLength,
		hookSize
	);

	float wreckingBallRadius = 2.f;

	ball = new WreckingBall(
		PxTransform(PxVec3(pose.p.x, ((floor(height / sectionLength) * sectionLength) + beamThickness + pose.p.y - wreckingBallRadius - (hookSize * .6f)) * .25f, hook->getMinExtension() * 2 + pose.p.z), pose.q),
		wreckingBallRadius,
		1.f
	);

	chain = new Chain((Actor*)hook, PxVec3(0.f, -(hookSize * .6f), hook->getMinExtension()), (Actor*)ball, PxVec3(0.f, wreckingBallRadius, 0.f));
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
	int numSections = floor(height / sectionLength);

	vector<PxVec3> cornerBeam = {
		PxVec3(-beamThickness / 2, 0.f, -beamThickness / 2),
		PxVec3(-beamThickness / 2, 0.f, beamThickness / 2),
		PxVec3(beamThickness / 2, 0.f, beamThickness / 2),
		PxVec3(beamThickness / 2, 0.f, -beamThickness / 2),

		PxVec3(-beamThickness / 2, numSections * sectionLength, -beamThickness / 2),
		PxVec3(-beamThickness / 2, numSections * sectionLength, beamThickness / 2),
		PxVec3(beamThickness / 2, numSections * sectionLength, beamThickness / 2),
		PxVec3(beamThickness / 2, numSections * sectionLength, -beamThickness / 2)
	};

	// Make the 4 cuboids that make up the corners of the shaft
	createShape(
		PhysicsEngine::CreateConvexMeshGeometry(cornerBeam, -baseSize / 2 + beamThickness / 2, 0.f, -baseSize / 2 + beamThickness / 2),
		1.0f
	);

	createShape(
		PhysicsEngine::CreateConvexMeshGeometry(cornerBeam, baseSize / 2 - beamThickness / 2, 0.f, -baseSize / 2 + beamThickness / 2),
		1.0f
	);

	createShape(
		PhysicsEngine::CreateConvexMeshGeometry(cornerBeam, baseSize / 2 - beamThickness / 2, 0.f, baseSize / 2 - beamThickness / 2),
		1.0f
	);

	createShape(
		PhysicsEngine::CreateConvexMeshGeometry(cornerBeam, -baseSize / 2 + beamThickness / 2, 0.f, baseSize / 2 - beamThickness / 2),
		1.0f
	);

	vector<PxVec3> sideBeam = {
		PxVec3(0.f, 0.f, -baseSize / 2 + beamThickness),
		PxVec3(0.f, 0.f, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, 0.f, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, 0.f, -baseSize / 2 + beamThickness),

		PxVec3(0.f, beamThickness, -baseSize / 2 + beamThickness),
		PxVec3(0.f, beamThickness, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, beamThickness, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, beamThickness, -baseSize / 2 + beamThickness)
	};

	float latOffset = (beamThickness / 2) / cos(atan((baseSize - (2 * beamThickness)) / (sectionLength - (2 * beamThickness))));
	vector<PxVec3> crossBrace = {
		PxVec3(0.f, 0.f, -baseSize / 2 + beamThickness),
		PxVec3(0.f, 0.f, -baseSize / 2 + beamThickness + latOffset),
		PxVec3(0.f, sectionLength - beamThickness - latOffset, baseSize / 2 - beamThickness),

		PxVec3(beamThickness, sectionLength - beamThickness - latOffset, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, 0.f, -baseSize / 2 + beamThickness + latOffset),
		PxVec3(beamThickness, 0.f, -baseSize / 2 + beamThickness),


		PxVec3(0.f, latOffset, -baseSize / 2 + beamThickness),
		PxVec3(0.f, sectionLength - beamThickness, baseSize / 2 - beamThickness - latOffset),
		PxVec3(0.f, sectionLength - beamThickness, baseSize / 2 - beamThickness),

		PxVec3(beamThickness, sectionLength - beamThickness, baseSize / 2 - beamThickness),
		PxVec3(beamThickness, sectionLength - beamThickness, baseSize / 2 - beamThickness - latOffset),
		PxVec3(beamThickness, latOffset, -baseSize / 2 + beamThickness)
	};

	for (int i = 0; i < numSections; i++)
	{
		// Side sections
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(sideBeam, -baseSize / 2, i * sectionLength, 0.f),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(sideBeam, 0.f, i * sectionLength, -baseSize / 2, PxVec3(0.f, PxPi / 2, 0.f)),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(sideBeam, baseSize / 2, i * sectionLength, 0.f, PxVec3(0.f, PxPi, 0.f)),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(sideBeam, 0.f, i * sectionLength, baseSize / 2, PxVec3(0.f, -PxPi / 2, 0.f)),
			1.0f
		);

		// Cross braces
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(crossBrace, -baseSize / 2, i * sectionLength + beamThickness, 0.f),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(crossBrace, 0.f, i * sectionLength + beamThickness, -baseSize / 2, PxVec3(0.f, PxPi / 2, 0.f)),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(crossBrace, baseSize / 2, i * sectionLength + beamThickness, 0.f, PxVec3(0.f, PxPi, 0.f)),
			1.0f
		);
		createShape(
			PhysicsEngine::CreateConvexMeshGeometry(crossBrace, 0.f, i * sectionLength + beamThickness, baseSize / 2, PxVec3(0.f, -PxPi / 2, 0.f)),
			1.0f
		);
	}

	// Top section
	createShape(
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2, 0.f, -baseSize / 2),
				PxVec3(-baseSize / 2, 0.f, baseSize / 2),
				PxVec3(baseSize / 2, 0.f, baseSize / 2),
				PxVec3(baseSize / 2, 0.f, -baseSize / 2),

				PxVec3(-baseSize / 2, beamThickness, -baseSize / 2),
				PxVec3(-baseSize / 2, beamThickness, baseSize / 2),
				PxVec3(baseSize / 2, beamThickness, baseSize / 2),
				PxVec3(baseSize / 2, beamThickness, -baseSize / 2)
			}
		), 0.f, numSections * sectionLength, 0.f),
		1.0f
	);

	setColour(PxVec3(1.f, 0.f, 0.f));
}

CraneArm::CraneArm(const PxTransform& pose, float baseSize, float beamThickness, float sectionLength, float length)
	: StaticActor(pose)
{
	// Front section
	int numSections_front = floor(length / sectionLength);

	vector<PxVec3> hCornerBeams_front = {
		PxVec3(-beamThickness / 2, 0.f, 0.f),
		PxVec3(-beamThickness / 2, 0.f, numSections_front * sectionLength + beamThickness),
		PxVec3(beamThickness / 2, 0.f, numSections_front * sectionLength + beamThickness),
		PxVec3(beamThickness / 2, 0.f, 0.f),

		PxVec3(-beamThickness / 2, beamThickness, 0.f),
		PxVec3(-beamThickness / 2, beamThickness, numSections_front * sectionLength + beamThickness),
		PxVec3(beamThickness / 2, beamThickness, numSections_front * sectionLength + beamThickness),
		PxVec3(beamThickness / 2, beamThickness, 0.f)
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_front, -baseSize / 2 + beamThickness / 2, 0.f, 0.f), 1.0f);
	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_front, baseSize / 2 - beamThickness / 2, 0.f, 0.f), 1.0f);
	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_front, 0.f, baseSize - beamThickness, 0.f), 1.0f);

	float latOffset = (beamThickness / 2) / cos(atan((baseSize - (2 * beamThickness)) / (sectionLength - (2 * beamThickness))));

	float diagHorzOffset_Outer = latOffset * cos(atan(baseSize / (baseSize / 2)));
	float diagVertOffset_Outer = latOffset * sin(atan(baseSize / (baseSize / 2)));

	float diagHorzOffset_Inner = latOffset * cos(atan((baseSize - beamThickness) / (baseSize / 2 - beamThickness)));
	float diagVertOffset_Inner = latOffset * sin(atan((baseSize - beamThickness) / (baseSize / 2 - beamThickness)));

	for (int i = 0; i < numSections_front + 1; i++)
	{
		// Create the side beams
		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2 + beamThickness / 2, 0.f, 0.f),
				PxVec3(-baseSize / 2 + beamThickness / 2, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, 0.f, 0.f),

				PxVec3(-baseSize / 2 + beamThickness / 2, beamThickness, 0.f),
				PxVec3(-baseSize / 2 + beamThickness / 2, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, beamThickness, 0.f)
			}
		), 0.f, 0.f, i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, 0.f),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness),
				PxVec3(0.f, baseSize - beamThickness, 0.f),
				PxVec3(0.f, baseSize - beamThickness, beamThickness),

				PxVec3(-baseSize / 2, beamThickness, 0.f),
				PxVec3(-baseSize / 2, beamThickness, beamThickness),
				PxVec3(-beamThickness / 2, baseSize, 0.f),
				PxVec3(-beamThickness / 2, baseSize, beamThickness)

			}
		), 0.f, 0.f, i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(0.f, baseSize - beamThickness, 0.f),
				PxVec3(0.f, baseSize - beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, 0.f),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness),

				PxVec3(beamThickness / 2, baseSize, 0.f),
				PxVec3(beamThickness / 2, baseSize, beamThickness),
				PxVec3(baseSize / 2, beamThickness, 0.f),
				PxVec3(baseSize / 2, beamThickness, beamThickness)
			}
		), 0.f, 0.f, i * sectionLength), 1.0f);

		if (i == numSections_front) continue;

		// Create the cross braces
		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(baseSize / 2 - beamThickness - latOffset, 0.f, beamThickness),
				PxVec3(-baseSize / 2 + beamThickness, 0.f, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + beamThickness, 0.f, sectionLength),

				PxVec3(baseSize / 2 - beamThickness, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, 0.f, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness + latOffset, 0.f, sectionLength),


				PxVec3(baseSize / 2 - beamThickness - latOffset, beamThickness, beamThickness),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, sectionLength),

				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness + latOffset, beamThickness, sectionLength),
			}
			), 0.f, 0.f, i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2, beamThickness, beamThickness),
				PxVec3(-baseSize / 2, beamThickness, beamThickness + latOffset),
				PxVec3(-beamThickness / 2 - diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, sectionLength),

				PxVec3(-beamThickness / 2 - diagHorzOffset_Inner, baseSize - beamThickness - diagVertOffset_Inner, sectionLength),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness),


				PxVec3(-baseSize / 2 + diagHorzOffset_Outer, beamThickness + diagVertOffset_Outer, beamThickness),
				PxVec3(-beamThickness / 2, baseSize, sectionLength - latOffset),
				PxVec3(-beamThickness / 2, baseSize, sectionLength),

				PxVec3(0.f, baseSize - beamThickness, sectionLength),
				PxVec3(0.f, baseSize - beamThickness, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, beamThickness)
			}
		), 0.f, 0.f, i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(diagHorzOffset_Inner, baseSize - beamThickness - diagVertOffset_Inner, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, sectionLength - latOffset),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, sectionLength),

				PxVec3(baseSize / 2, beamThickness, sectionLength),
				PxVec3(baseSize / 2, beamThickness, sectionLength - latOffset),
				PxVec3(beamThickness / 2 + diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, beamThickness),


				PxVec3(0.f, baseSize - beamThickness, beamThickness),
				PxVec3(0.f, baseSize - beamThickness, beamThickness + latOffset),
				PxVec3(baseSize / 2 - diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, sectionLength),

				PxVec3(baseSize / 2 - diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, sectionLength),
				PxVec3(beamThickness / 2 + diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, beamThickness),
				PxVec3(beamThickness / 2, baseSize, beamThickness)
			}
		), 0.f, 0.f, i * sectionLength), 1.0f);
	}

	// Back section (counterweight)
	int numSections_back = floor((float)numSections_front / 3);

	vector<PxVec3> hCornerBeams_back = {
		PxVec3(-beamThickness / 2, 0.f, 0.f),
		PxVec3(-beamThickness / 2, 0.f, -numSections_back * sectionLength),
		PxVec3(beamThickness / 2, 0.f, -numSections_back * sectionLength),
		PxVec3(beamThickness / 2, 0.f, 0.f),

		PxVec3(-beamThickness / 2, beamThickness, 0.f),
		PxVec3(-beamThickness / 2, beamThickness, -numSections_back * sectionLength),
		PxVec3(beamThickness / 2, beamThickness, -numSections_back * sectionLength),
		PxVec3(beamThickness / 2, beamThickness, 0.f)
	};

	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_back, -baseSize / 2 + beamThickness / 2, 0.f, 0.f), 1.0f);
	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_back, baseSize / 2 - beamThickness / 2, 0.f, 0.f), 1.0f);
	createShape(PhysicsEngine::CreateConvexMeshGeometry(hCornerBeams_back, 0.f, baseSize - beamThickness, 0.f), 1.0f);

	for (int i = 1; i < numSections_back + 1; i++)
	{
		// Create the side beams
		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2 + beamThickness / 2, 0.f, 0.f),
				PxVec3(-baseSize / 2 + beamThickness / 2, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, 0.f, 0.f),

				PxVec3(-baseSize / 2 + beamThickness / 2, beamThickness, 0.f),
				PxVec3(-baseSize / 2 + beamThickness / 2, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness / 2, beamThickness, 0.f)
			}
		), 0.f, 0.f, -i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, 0.f),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness),
				PxVec3(0.f, baseSize - beamThickness, 0.f),
				PxVec3(0.f, baseSize - beamThickness, beamThickness),

				PxVec3(-baseSize / 2, beamThickness, 0.f),
				PxVec3(-baseSize / 2, beamThickness, beamThickness),
				PxVec3(-beamThickness / 2, baseSize, 0.f),
				PxVec3(-beamThickness / 2, baseSize, beamThickness)

			}
		), 0.f, 0.f, -i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(0.f, baseSize - beamThickness, 0.f),
				PxVec3(0.f, baseSize - beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, 0.f),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness),

				PxVec3(beamThickness / 2, baseSize, 0.f),
				PxVec3(beamThickness / 2, baseSize, beamThickness),
				PxVec3(baseSize / 2, beamThickness, 0.f),
				PxVec3(baseSize / 2, beamThickness, beamThickness)
			}
		), 0.f, 0.f, -i * sectionLength), 1.0f);

		// Create the cross braces
		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(baseSize / 2 - beamThickness - latOffset, 0.f, beamThickness),
				PxVec3(-baseSize / 2 + beamThickness, 0.f, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + beamThickness, 0.f, sectionLength),

				PxVec3(baseSize / 2 - beamThickness, 0.f, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, 0.f, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness + latOffset, 0.f, sectionLength),


				PxVec3(baseSize / 2 - beamThickness - latOffset, beamThickness, beamThickness),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, sectionLength),

				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness + latOffset, beamThickness, sectionLength),
			}
			), 0.f, 0.f, -i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(-baseSize / 2, beamThickness, beamThickness),
				PxVec3(-baseSize / 2, beamThickness, beamThickness + latOffset),
				PxVec3(-beamThickness / 2 - diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, sectionLength),

				PxVec3(-beamThickness / 2 - diagHorzOffset_Inner, baseSize - beamThickness - diagVertOffset_Inner, sectionLength),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness + latOffset),
				PxVec3(-baseSize / 2 + beamThickness, beamThickness, beamThickness),


				PxVec3(-baseSize / 2 + diagHorzOffset_Outer, beamThickness + diagVertOffset_Outer, beamThickness),
				PxVec3(-beamThickness / 2, baseSize, sectionLength - latOffset),
				PxVec3(-beamThickness / 2, baseSize, sectionLength),

				PxVec3(0.f, baseSize - beamThickness, sectionLength),
				PxVec3(0.f, baseSize - beamThickness, sectionLength - latOffset),
				PxVec3(-baseSize / 2 + diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, beamThickness)
			}
		), 0.f, 0.f, -i * sectionLength), 1.0f);

		createShape(PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(diagHorzOffset_Inner, baseSize - beamThickness - diagVertOffset_Inner, beamThickness),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, sectionLength - latOffset),
				PxVec3(baseSize / 2 - beamThickness, beamThickness, sectionLength),

				PxVec3(baseSize / 2, beamThickness, sectionLength),
				PxVec3(baseSize / 2, beamThickness, sectionLength - latOffset),
				PxVec3(beamThickness / 2 + diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, beamThickness),


				PxVec3(0.f, baseSize - beamThickness, beamThickness),
				PxVec3(0.f, baseSize - beamThickness, beamThickness + latOffset),
				PxVec3(baseSize / 2 - diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, sectionLength),

				PxVec3(baseSize / 2 - diagHorzOffset_Inner, beamThickness + diagVertOffset_Inner, sectionLength),
				PxVec3(beamThickness / 2 + diagHorzOffset_Outer, baseSize - diagVertOffset_Outer, beamThickness),
				PxVec3(beamThickness / 2, baseSize, beamThickness)
			}
		), 0.f, 0.f, -i * sectionLength), 1.0f);
	}

	setColour(PxVec3(1.f, 0.f, 0.f));
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
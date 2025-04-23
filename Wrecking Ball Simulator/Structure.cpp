#include "Structure.h"
#include "PhysicsEngine.h"
#include "Helpful.h"

Structure::Structure(const PxTransform& pose, int floors, int width, int depth)
{
	float supportSeparation = 10.f;
	float floorHeight = 10.f;

	PxVec3 minusMinusCorner = PxVec3(0.f, 0.f, 0.f);

	if (width % 2 == 0)
		minusMinusCorner.x = -((width / static_cast<float>(2)) * supportSeparation) + supportSeparation / 2;
	else
		minusMinusCorner.x = -((width / static_cast<float>(2)) * supportSeparation);

	if (depth % 2 == 0)
		minusMinusCorner.z = -((depth / static_cast<float>(2)) * supportSeparation) + supportSeparation / 2;
	else
		minusMinusCorner.z = -((depth / static_cast<float>(2)) * supportSeparation);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			// If the Foundation is not on the edge of the structure, skip it
			if (i != 0 && i != width - 1 && j != 0 && j != depth - 1)
				continue;

			PxVec3 position = minusMinusCorner + PxVec3(i * supportSeparation, 0.f, j * supportSeparation);
			PxTransform transform = PxTransform(pose.p + position);
			Foundation* foundation = new Foundation(transform);
			foundations.push_back(foundation);

			Actor* ancorActor = foundation;

			for (int k = 0; k < floors / 2 + floors % 2; k++)
			{
				SupportPillar* supportPillar = new SupportPillar(foundation, ancorActor, k, floorHeight);
				supportPillars.push_back(supportPillar);

				ancorActor = supportPillar;
			}
		}
	}

	PxVec3 floorPosOffset = pose.p + PxVec3(0.f, 0.f, 0.f);
	if (width % 2 == 1)
		floorPosOffset.x -= supportSeparation / 2;
	if (depth % 2 == 1)
		floorPosOffset.z -= supportSeparation / 2;

	for (int i = 0; i < floors; i++)
	{
		StructureFloor* floor = new StructureFloor(
			PxTransform(floorPosOffset + PxVec3(0.f, 1.f + i * floorHeight, 0.f), pose.q),
			((width - 1) * supportSeparation) - 1.f,
			((depth - 1) * supportSeparation) - 1.f,
			1.f,
			i,
			floorHeight,
			foundations,
			supportPillars
		);
		this->floors.push_back(floor);
	}

	for (int i = 0; i < floors; i++)
	{
		int pillarX = 0;
		int pillarZ = 0;

		SupportPillar*** supportPillarGrid = new SupportPillar * *[width];
		for (int j = 0; j < width; j++)
				supportPillarGrid[j] = nullptr;

		for (int j = 0; j < foundations.size(); j++)
		{
			if (supportPillarGrid[pillarX] == nullptr)
				supportPillarGrid[pillarX] = new SupportPillar * [depth];

			supportPillarGrid[pillarX][pillarZ] = supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(j) * (static_cast<unsigned long long>(floors / 2) + floors % 2) + (i / 2)];

			pillarX++;

			if (pillarX == width)
			{
				pillarX = 0;
				pillarZ++;
			}
			else if (pillarZ != 0 && pillarZ != depth - 1)
				pillarX += width - 2;
		}

		pillarX = 0;
		pillarZ = 0;

		for (int j = 0; j < foundations.size(); j++)
		{			
			int attachment1X = 0;
			int attachment1Z = 0;

			if (pillarZ == 0 && pillarX != width - 1)
			{
				attachment1X = pillarX + 1;
				attachment1Z = pillarZ;
			}
			else if (pillarZ == depth - 1 && pillarX != 0)
			{
				attachment1X = pillarX - 1;
				attachment1Z = pillarZ;
			}
			else if (pillarX == 0 && pillarZ != 0)
			{
				attachment1X = pillarX;
				attachment1Z = pillarZ - 1;
			}
			else if (pillarX == width - 1 && pillarZ != depth - 1)
			{
				attachment1X = pillarX;
				attachment1Z = pillarZ + 1;
			}

			SupportPillar* attachment0 = supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(j) * (static_cast<unsigned long long>(floors / 2) + floors % 2) + (i / 2)];
			SupportPillar* attachment1 = supportPillarGrid[attachment1X] != nullptr ? supportPillarGrid[attachment1X][attachment1Z] : nullptr;

			vector<PxVec3> jointLocations;
			vector<PxVec3> attachment0JointLocations;
			vector<PxVec3> attachment1JointLocations;

			PxVec3 size = PxVec3(0.f, 0.f, 0.f);

			float xAdjustment = 0.f;
			if (pillarX == 0 && pillarZ != 0)
			{
				// 0
				xAdjustment = -supportSeparation / 2;
				size = PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, floorHeight / 2, foundations[0]->getSize() / 2);
				
				jointLocations = {
					PxVec3(-supportSeparation / 2 + foundations[0]->getSize() / 2, floorHeight / 2, 0.f),
					PxVec3(-supportSeparation / 2 + foundations[0]->getSize() / 2, -floorHeight / 2, 0.f),
					PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, -floorHeight / 2, 0.f),
					PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, floorHeight / 2, 0.f)
				};
				attachment0JointLocations = {
					PxVec3(-foundations[0]->getSize() / 2, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f),
					PxVec3(-foundations[0]->getSize() / 2, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f)
				};
				attachment1JointLocations = {
					PxVec3(foundations[0]->getSize() / 2, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f),
					PxVec3(foundations[0]->getSize() / 2, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f)
				};
			}
			else if (pillarX == width - 1 && pillarZ != depth - 1)
			{
				// 180
				xAdjustment = supportSeparation / 2;
				size = PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, floorHeight / 2, foundations[0]->getSize() / 2);

				jointLocations = {
					PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, floorHeight / 2, 0.f),
					PxVec3(supportSeparation / 2 - foundations[0]->getSize() / 2, -floorHeight / 2, 0.f),
					PxVec3(-supportSeparation / 2 + foundations[0]->getSize() / 2, -floorHeight / 2, 0.f),
					PxVec3(-supportSeparation / 2 + foundations[0]->getSize() / 2, floorHeight / 2, 0.f)
				};
				attachment0JointLocations = {
					PxVec3(foundations[0]->getSize() / 2, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f),
					PxVec3(foundations[0]->getSize() / 2, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f)
				};
				attachment1JointLocations = {
					PxVec3(-foundations[0]->getSize() / 2, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f),
					PxVec3(-foundations[0]->getSize() / 2, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), 0.f)
				};
			}

			float zAdjustment = 0.f;
			if (pillarZ == 0 && pillarX != width - 1)
			{
				// -90
				zAdjustment = supportSeparation / 2;
				size = PxVec3(foundations[0]->getSize() / 2, floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2);

				jointLocations = {
					PxVec3(0.f, floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2, -supportSeparation / 2 + foundations[0]->getSize() / 2),
					PxVec3(0.f, floorHeight / 2, -supportSeparation / 2 + foundations[0]->getSize() / 2),
				};
				attachment0JointLocations = {
					PxVec3(0.f, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), foundations[0]->getSize() / 2),
					PxVec3(0.f, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), foundations[0]->getSize() / 2)
				};
				attachment1JointLocations = {
					PxVec3(0.f, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), -foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), -foundations[0]->getSize() / 2)
				};
			}
			else if (pillarZ == depth - 1 && pillarX != 0)
			{
				// 90
				zAdjustment = -supportSeparation / 2;
				size = PxVec3(foundations[0]->getSize() / 2, floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2);

				jointLocations = {
					PxVec3(0.f, floorHeight / 2, -supportSeparation / 2 + foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2, -supportSeparation / 2 + foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2),
					PxVec3(0.f, floorHeight / 2, supportSeparation / 2 - foundations[0]->getSize() / 2),
				};
				attachment0JointLocations = {
					PxVec3(0.f, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), -foundations[0]->getSize() / 2),
					PxVec3(0.f, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), -foundations[0]->getSize() / 2)
				};
				attachment1JointLocations = {
					PxVec3(0.f, floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), foundations[0]->getSize() / 2),
					PxVec3(0.f, -floorHeight / 2 + (i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2), foundations[0]->getSize() / 2)
				};
			}

			PxVec3 pos = attachment0->getPxActor()->getGlobalPose().p + PxVec3(xAdjustment, i % 2 == 0 ? -floorHeight / 2 : floorHeight / 2, zAdjustment);

			Window* window = new Window(
				PxTransform(pos),
				size,
				jointLocations,
				attachment0JointLocations,
				attachment1JointLocations,
				attachment0,
				attachment1
			);
			windows.push_back(window);

			pillarX++;

			if (pillarX == width)
			{
				pillarX = 0;
				pillarZ++;
			}
			else if (pillarZ != 0 && pillarZ != depth - 1)
				pillarX += width - 2;
		}

	}
}

Structure::~Structure()
{
	for (Foundation* foundation : foundations)
		delete foundation;
	for (SupportPillar* supportPillar : supportPillars)
		delete supportPillar;
	for (StructureFloor* floor : floors)
		delete floor;
	for (Window* window : windows)
		delete window;
	for (WindowShards* windowShards : windowShardsCollection)
		delete windowShards;
}

void Structure::Update(float deltaTime)
{
	for (SupportPillar* supportPillar : supportPillars)
		supportPillar->Update(deltaTime);
	for (StructureFloor* floor : floors)
		floor->Update(deltaTime);
	for (Window* window : windows)
		window->Update(deltaTime);

	// Check for each broken window if it has broken into shards
	for (Window* window : windows)
	{
		if (!window->IsShattered() && window->IsBroken())
		{
			WindowShards* shards = new WindowShards(window);
			windowShardsCollection.push_back(shards);

			window->Shatter();
		}
	}

	for (WindowShards* windowShards : windowShardsCollection)
		windowShards->Update(deltaTime);
}

void Structure::Collapse()
{
	for (int i = 0; i < supportPillars.size(); i++)
		supportPillars[i]->Break();
}

vector<Actor*> Structure::getActors()
{
	vector<Actor*> actors;
	for (Foundation* foundation : foundations)
		actors.push_back(foundation);
	for (SupportPillar* supportPillar : supportPillars)
		actors.push_back(supportPillar);
	for (StructureFloor* floor : floors)
		actors.push_back(floor);
	for (Window* window : windows)
		actors.push_back(window);
	for(WindowShards* windowShards : windowShardsCollection)
		for (Actor* shard : windowShards->getShards())
			actors.push_back(shard);

	return actors;
}

vector<Actor*> Structure::getMissingActors()
{
	vector<Actor*> missingActors;
	for (WindowShards* windowShards : windowShardsCollection)
	{
		if (windowShards->AddedToScene)
			continue;

		for (Actor* shard : windowShards->getShards())
			missingActors.push_back(shard);

		windowShards->AddedToScene = true;
	}

	return missingActors;
}

int Structure::getScore()
{
	int score = 0;
	for (StructureMaterial* material : supportPillars)
		if (material->IsBroken())
			score += 300;
	for (StructureMaterial* material : floors)
		if (material->IsBroken())
			score += 500;
	for (StructureMaterial* material : windows)
		if (material->IsBroken())
			score += 100;

	return score;
}

int Structure::getMaxScore()
{
	int score = 0;
	for (StructureMaterial* material : supportPillars)
		score += 300;
	for (StructureMaterial* material : floors)
		score += 500;
	for (StructureMaterial* material : windows)
		score += 100;

	return score;
}

Foundation::Foundation(const PxTransform& pose, float foundationSize)
	: StaticActor(PxTransform(pose.p + PxVec3(0.f, 0.5f, 0.f), pose.q)), foundationSize(foundationSize)
{
	createShape(PxBoxGeometry(foundationSize, 0.5f, foundationSize));
	setColour(Helpful::RGBtoScalar(0.6f, 0.4f, 0.2f)); // brown wood
}

SupportPillar::SupportPillar(Foundation* foundation, Actor* ancorActor, int floor, float floorHeight)
	: StructureMaterial(PxTransform(foundation->getPxActor()->getGlobalPose().p + PxVec3(0.f, .5f + (floor * floorHeight) * 2 + floorHeight, 0.f)))
{
	breakForce = 200.f;

	AddAnchor(ancorActor);
	createShape(PxBoxGeometry(foundation->getSize() / 2, floorHeight, foundation->getSize() / 2), 1.0f);

	// Check if the ancorActor is a foundation
	if (Foundation* anchor = dynamic_cast<Foundation*>(ancorActor))
	{
		// Create 4 joints to connect the pillar to the foundation
		RevoluteJoint* joint0 = new RevoluteJoint(anchor, PxTransform(PxVec3(-foundation->getSize() * .5f, .5f, -foundation->getSize() * .5f)), this, PxTransform(PxVec3(-foundation->getSize() * .5f, -floorHeight, -foundation->getSize() * .5f)));
		joint0->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint1 = new RevoluteJoint(anchor, PxTransform(PxVec3(-foundation->getSize() * .5f, .5f, foundation->getSize() * .5f)), this, PxTransform(PxVec3(-foundation->getSize() * .5f, -floorHeight, foundation->getSize() * .5f)));
		joint1->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint2 = new RevoluteJoint(anchor, PxTransform(PxVec3(foundation->getSize() * .5f, .5f, foundation->getSize() * .5f)), this, PxTransform(PxVec3(foundation->getSize() * .5f, -floorHeight, foundation->getSize() * .5f)));
		joint2->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint3 = new RevoluteJoint(anchor, PxTransform(PxVec3(foundation->getSize() * .5f, .5f, -foundation->getSize() * .5f)), this, PxTransform(PxVec3(foundation->getSize() * .5f, -floorHeight, -foundation->getSize() * .5f)));
		joint3->setLimits(-PxPi / 4, PxPi / 4);

		AddJoint(joint0, anchor);
		AddJoint(joint1, anchor);
		AddJoint(joint2, anchor);
		AddJoint(joint3, anchor);
	}
	else if (SupportPillar* anchor = dynamic_cast<SupportPillar*>(ancorActor))
	{
		// Create 4 joints to connect the pillar to the pillar
		RevoluteJoint* joint0 = new RevoluteJoint(anchor, PxTransform(PxVec3(-foundation->getSize() * .5f, floorHeight, -foundation->getSize() * .5f)), this, PxTransform(PxVec3(-foundation->getSize() * .5f, -floorHeight, -foundation->getSize() * .5f)));
		joint0->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint1 = new RevoluteJoint(anchor, PxTransform(PxVec3(-foundation->getSize() * .5f, floorHeight, foundation->getSize() * .5f)), this, PxTransform(PxVec3(-foundation->getSize() * .5f, -floorHeight, foundation->getSize() * .5f)));
		joint1->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint2 = new RevoluteJoint(anchor, PxTransform(PxVec3(foundation->getSize() * .5f, floorHeight, foundation->getSize() * .5f)), this, PxTransform(PxVec3(foundation->getSize() * .5f, -floorHeight, foundation->getSize() * .5f)));
		joint2->setLimits(-PxPi / 4, PxPi / 4);
		RevoluteJoint* joint3 = new RevoluteJoint(anchor, PxTransform(PxVec3(foundation->getSize() * .5f, floorHeight, -foundation->getSize() * .5f)), this, PxTransform(PxVec3(foundation->getSize() * .5f, -floorHeight, -foundation->getSize() * .5f)));
		joint3->setLimits(-PxPi / 4, PxPi / 4);

		AddJoint(joint0, anchor);
		AddJoint(joint1, anchor);
		AddJoint(joint2, anchor);
		AddJoint(joint3, anchor);
	}

	setColour(Helpful::RGBtoScalar(0.5f, 0.5f, 0.5f)); // Grey color
	setName("Pillar");
}

void SupportPillar::OnCollision(Collider* other)
{
	// Wait for the House to settle before checking for collisions
	if (PhysicsEngine::getUptime() < 3.f) return;

	if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(other))
	{
		PxVec3 velocity = ((PxRigidDynamic*)getPxActor())->getLinearVelocity();
		PxVec3 colliderVelocity = ((PxRigidDynamic*)material->getPxActor())->getLinearVelocity();

		PxVec3 impulse = PxVec3(
			((PxReal)velocity.x - (PxReal)colliderVelocity.x) * ((PxRigidDynamic*)getPxActor())->getMass(),
			((PxReal)velocity.y - (PxReal)colliderVelocity.y) * ((PxRigidDynamic*)getPxActor())->getMass(),
			((PxReal)velocity.z - (PxReal)colliderVelocity.z) * ((PxRigidDynamic*)getPxActor())->getMass()
		);

		float impulseMagnitude = impulse.magnitude();
		
		if (impulseMagnitude > material->getBreakForce())
			material->Break();
	}
}

StructureFloor::StructureFloor(const PxTransform& pose, float width, float depth, float height, int floor, float floorHeight, vector<Foundation*> foundations, vector<SupportPillar*> supportPillars)
	: StructureMaterial(PxTransform(pose.p + PxVec3(0.f, height / 2, 0.f), pose.q))
{
	breakForce = 500.f;

	createShape(PxBoxGeometry(width / 2, height / 2, depth / 2), 1.0f);

	int numPillarsZ = 0;

	float xPos = foundations[0]->getPxActor()->getGlobalPose().p.x;
	for (int i = 1; i < foundations.size(); i++)
		if (foundations[i]->getPxActor()->getGlobalPose().p.x != xPos)
		{
			numPillarsZ = i;
			break;
		}

	int numPillarsX = (foundations.size() - (static_cast<unsigned long long>(numPillarsZ) * 2)) / 2 + 2;

	float pillarSeparation = (width + 1.f) / (numPillarsZ - 1);
	int layers = supportPillars.size() / foundations.size();

	int pillarX = 0;
	int pillarZ = 0;
	float yValueAdjustment = floor % 2 == 0 ? -floorHeight + height / 2 : height / 2;

	for (int i = 0; i < foundations.size(); i++)
	{
		PxVec2 pillarOffset = PxVec2(0.f, 0.f);

		if (pillarX == 0)
			pillarOffset.y = foundations[i]->getSize() / 2;
		else if (pillarX == numPillarsX - 1)
			pillarOffset.y = -foundations[i]->getSize() / 2;

		if (pillarZ == 0)
			pillarOffset.x = foundations[i]->getSize() / 2;
		else if (pillarZ == numPillarsZ - 1)
			pillarOffset.x = -foundations[i]->getSize() / 2;

		PxVec2 floorOffset = PxVec2(
			(-depth / 2)
			+ pillarSeparation * pillarZ
			+ (pillarZ == 0 ? 0.f : -foundations[i]->getSize() / 2)
			+ (pillarZ == numPillarsZ - 1 ? -foundations[i]->getSize() / 2 : 0.f),
			(-width / 2)
			+ pillarSeparation * pillarX
			+ (pillarX == 0 ? 0.f : -foundations[i]->getSize() / 2)
			+ (pillarX == numPillarsX - 1 ? -foundations[i]->getSize() / 2 : 0.f)
		);

		// Corner pillars
		if ((pillarX == 0 && pillarZ == 0) || (pillarX == 0 && pillarZ == numPillarsZ - 1) || (pillarX == numPillarsX - 1 && pillarZ == 0) || (pillarX == numPillarsX - 1 && pillarZ == numPillarsZ - 1))
		{
			RevoluteJoint* joint0 = new RevoluteJoint(
				supportPillars[i * layers + (floor / 2)],
				PxTransform(PxVec3(pillarOffset.x, yValueAdjustment, pillarOffset.y)),
				this,
				PxTransform(PxVec3(floorOffset.x, 0.f, floorOffset.y))
			);
			joint0->setLimits(-PxPi / 4, PxPi / 4);

			AddJoint(joint0, supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]);

			AddAnchor(supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]);
			supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]->AddJoint(joint0, this);
		}
		// Edge pillars
		else
		{
			RevoluteJoint* joint0 = new RevoluteJoint(
				supportPillars[i * layers + (floor / 2)],
				PxTransform(PxVec3(pillarOffset.x, yValueAdjustment, pillarOffset.y)),
				this,
				PxTransform(PxVec3(floorOffset.x, 0.f, floorOffset.y))
			);
			joint0->setLimits(-PxPi / 4, PxPi / 4);

			AddJoint(joint0, supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]);

			AddAnchor(supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]);
			supportPillars[static_cast<vector<SupportPillar*, allocator<SupportPillar*>>::size_type>(i) * layers + (floor / 2)]->AddJoint(joint0, this);
		}

		pillarX++;

		if (pillarX == numPillarsX)
		{
			pillarX = 0;
			pillarZ++;
		}
		else if (pillarZ != 0 && pillarZ != numPillarsZ - 1)
			pillarX += numPillarsX - 2;
	}

	setColour(Helpful::RGBtoScalar(0.5f, 0.5f, 0.5f)); // Grey color
	setName("Floor");
}

void StructureFloor::OnCollision(Collider* other)
{
	// Wait for the House to settle before checking for collisions
	if (PhysicsEngine::getUptime() < 3.f) return;

	if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(other))
	{
		PxVec3 velocity = ((PxRigidDynamic*)getPxActor())->getLinearVelocity();
		PxVec3 colliderVelocity = ((PxRigidDynamic*)material->getPxActor())->getLinearVelocity();

		// Use the other material's mass to calculate the impulse (due to the floor being so heavy)
		PxVec3 impulse = PxVec3(
			((PxReal)velocity.x - (PxReal)colliderVelocity.x) * ((PxRigidDynamic*)material->getPxActor())->getMass(),
			((PxReal)velocity.y - (PxReal)colliderVelocity.y) * ((PxRigidDynamic*)material->getPxActor())->getMass(),
			((PxReal)velocity.z - (PxReal)colliderVelocity.z) * ((PxRigidDynamic*)material->getPxActor())->getMass()
		);

		float impulseMagnitude = impulse.magnitude();

		if (impulseMagnitude > material->getBreakForce())
			material->Break();
	}
}

Window::Window(const PxTransform& pose, PxVec3 size, vector<PxVec3> jointLocations, vector<PxVec3> attachment0JointLocations, vector<PxVec3> attachment1JointLocations, Actor* attachment0, Actor* attachment1)
	: StructureMaterial(pose), size(size)
{
	breakForce = 100.f;

	createShape(PxBoxGeometry(size), .1f);

	// Create joints to connect the window to the House
	for (int i = 0; i < jointLocations.size(); i++)
	{
		RevoluteJoint* joint;
		if (i < jointLocations.size() / 2)
			joint = new RevoluteJoint(this, PxTransform(jointLocations[i]), attachment1, PxTransform(attachment1JointLocations[i]));
		else
			joint = new RevoluteJoint(this, PxTransform(jointLocations[i]), attachment0, PxTransform(attachment0JointLocations[i - jointLocations.size() / 2]));

		joint->setLimits(-PxPi / 4, PxPi / 4);

		AddJoint(joint, attachment0);
		AddJoint(joint, attachment1);

		AddAnchor(attachment0);
		AddAnchor(attachment1);
	}

	// Blue window
	setColour(Helpful::RGBtoScalar(119.f, 199.f, 190.f));
	setName("Window");
}

void Window::OnCollision(Collider* other)
{
	// Wait for the House to settle before checking for collisions
	if (PhysicsEngine::getUptime() < 3.f) return;

	if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(other))
	{
		PxVec3 velocity = ((PxRigidDynamic*)getPxActor())->getLinearVelocity();
		PxVec3 colliderVelocity = ((PxRigidDynamic*)material->getPxActor())->getLinearVelocity();

		PxVec3 impulse = PxVec3(
			((PxReal)velocity.x - (PxReal)colliderVelocity.x) * ((PxRigidDynamic*)getPxActor())->getMass(),
			((PxReal)velocity.y - (PxReal)colliderVelocity.y) * ((PxRigidDynamic*)getPxActor())->getMass(),
			((PxReal)velocity.z - (PxReal)colliderVelocity.z) * ((PxRigidDynamic*)getPxActor())->getMass()
		);

		float impulseMagnitude = impulse.magnitude();

		// Only Windows can break other windows
		if (impulseMagnitude > material->getBreakForce() && dynamic_cast<Window*>(material))
			material->Break();
	}
}

void Window::CustomBreak()
{
	// get the actor's position, rotation, velocity, and angular velocity
	breakTransform = getPxActor()->getGlobalPose();
	breakVelocity = ((PxRigidDynamic*)getPxActor())->getLinearVelocity();
	breakAngularVelocity = ((PxRigidDynamic*)getPxActor())->getAngularVelocity();

	// Delete the shapes and set it to Kinematic
	detachAllShapes();
	setKinematic(true);
}

WindowShards::WindowShards(Window* window)
	: window(window)
{
	// Determine which way round the glass is facing to get the best crack look
	PxVec3 size = window->getSize();
	bool isZFacing = size.x > size.z;

	// Create shards
	// 1
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? -size.x : -size.x, -size.y, isZFacing ? -size.z : -size.z),
				PxVec3(isZFacing ? -size.x : size.x, -size.y, isZFacing ? size.z : -size.z),
				PxVec3(isZFacing ? 0.f : size.x, -size.y, isZFacing ? size.z : 0.f),
				PxVec3(isZFacing ? 0.f : -size.x, -size.y, isZFacing ? -size.z : 0.f),

				PxVec3(isZFacing ? -size.x : -size.x, 0.f, isZFacing ? -size.z : -size.z),
				PxVec3(isZFacing ? -size.x : size.x, 0.f, isZFacing ? size.z : -size.z),
				PxVec3(isZFacing ? -size.x * .5f : size.x, 0.f, isZFacing ? size.z : -size.z * .5f),
				PxVec3(isZFacing ? -size.x * .5f : -size.x, 0.f, isZFacing ? -size.z : -size.z * .5f)
			}
			)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 2
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? 0.f : -size.x, -size.y, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, -size.y, isZFacing ? size.z : 0.f),
				PxVec3(isZFacing ? size.x : size.x, -size.y, isZFacing ? size.z : size.z),
				PxVec3(isZFacing ? size.x : -size.x, -size.y, isZFacing ? -size.z : size.z),

				PxVec3(isZFacing ? size.x : -size.x, size.y * .5f, isZFacing ? -size.z : size.z),
				PxVec3(isZFacing ? size.x : size.x, size.y * .5f, isZFacing ? size.z : size.z)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 3
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? 0.f : -size.x, -size.y, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, -size.y, isZFacing ? size.z : 0.f),

				PxVec3(isZFacing ? -size.x * .5f : size.x, 0.f, isZFacing ? size.z : -size.z * .5f),
				PxVec3(isZFacing ? -size.x * .5f : -size.x, 0.f, isZFacing ? -size.z : -size.z * .5f),
				PxVec3(isZFacing ? 0.f : -size.x, 0.f, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, 0.f, isZFacing ? size.z : 0.f)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 4
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? 0.f : -size.x, -size.y, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, -size.y, isZFacing ? size.z : 0.f),

				PxVec3(isZFacing ? 0.f : size.x, 0.f, isZFacing ? size.z : 0.f),
				PxVec3(isZFacing ? 0.f : -size.x, 0.f, isZFacing ? -size.z : 0.f),

				PxVec3(isZFacing ? size.x : size.x, size.y * .5f, isZFacing ? size.z : size.z),
				PxVec3(isZFacing ? size.x : -size.x, size.y * .5f, isZFacing ? -size.z : size.z)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 5
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? -size.x : -size.x, 0.f, isZFacing ? -size.z : -size.z),
				PxVec3(isZFacing ? -size.x : size.x, 0.f, isZFacing ? size.z : -size.z),
				PxVec3(isZFacing ? -size.x * .5f : size.x, 0.f, isZFacing ? size.z : -size.z * .5f),
				PxVec3(isZFacing ? -size.x * .5f : -size.x, 0.f, isZFacing ? -size.z : -size.z * .5f),

				PxVec3(isZFacing ? -size.x : size.x, size.y, isZFacing ? size.z : -size.z),
				PxVec3(isZFacing ? -size.x : -size.x, size.y, isZFacing ? -size.z : -size.z)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 6
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? -size.x * .5f : -size.x, 0.f, isZFacing ? -size.z : -size.z * .5f),
				PxVec3(isZFacing ? -size.x * .5f : size.x, 0.f, isZFacing ? size.z : -size.z * .5f),

				PxVec3(isZFacing ? -size.x : size.x, size.y, isZFacing ? size.z : -size.z),
				PxVec3(isZFacing ? -size.x : -size.x, size.y, isZFacing ? -size.z : -size.z),
				PxVec3(isZFacing ? 0.f : size.x, size.y, isZFacing ? size.z : 0.f),
				PxVec3(isZFacing ? 0.f : -size.x, size.y, isZFacing ? -size.z : 0.f)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));

	// 7
	shards.push_back(new WindowShard(
		window->getBreakTransform(),
		window->getBreakVelocity(),
		window->getBreakAngularVelocity(),
		PhysicsEngine::CreateConvexMeshGeometry(vector<PxVec3>(
			{
				PxVec3(isZFacing ? -size.x * .5f : -size.x, 0.f, isZFacing ? -size.z : -size.z * .5f),
				PxVec3(isZFacing ? -size.x * .5f : size.x, 0.f, isZFacing ? size.z : -size.z * .5f),
				PxVec3(isZFacing ? 0.f : -size.x, 0.f, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, 0.f, isZFacing ? size.z : 0.f),

				PxVec3(isZFacing ? size.x : size.x, size.y * .5f, isZFacing ? size.z : size.z),
				PxVec3(isZFacing ? size.x : -size.x, size.y * .5f, isZFacing ? -size.z : size.z),

				PxVec3(isZFacing ? 0.f : -size.x, size.y, isZFacing ? -size.z : 0.f),
				PxVec3(isZFacing ? 0.f : size.x, size.y, isZFacing ? size.z : 0.f),
				PxVec3(isZFacing ? size.x : size.x, size.y, isZFacing ? size.z : size.z),
				PxVec3(isZFacing ? size.x : -size.x, size.y, isZFacing ? -size.z : size.z)
			}
		)),
		1.f + (rand() % 100) / 100.f * 2.f
	));
}

void WindowShards::Update(float deltaTime)
{
	for (WindowShard* shard : shards)
		shard->CustomUpdate(deltaTime);
}

vector<Actor*> WindowShards::getShards()
{
	vector<Actor*> shardActors;
	for (WindowShard* shard : shards)
		shardActors.push_back(shard);

	return shardActors;
}

WindowShard::WindowShard(const PxTransform& pose, const PxVec3& velocity, const PxVec3& angularVelocity, const PxConvexMeshGeometry& mesh, float lifespan)
	: DynamicActor(pose), lifespan(lifespan)
{
	createShape(mesh, .1f);

	// Set the shard's velocity and angular velocity
	((PxRigidDynamic*)getPxActor())->setLinearVelocity(velocity);
	((PxRigidDynamic*)getPxActor())->setAngularVelocity(angularVelocity);

	// Marginally whiter than the window to make it look like broken glass
	float whiteAdjustment = .2f + (rand() % 100) / 100.f * .15f;

	setColour(PxVec3(.2f + whiteAdjustment, .5f + whiteAdjustment, .8f + whiteAdjustment));
	setName("WindowShard");
}

void WindowShard::CustomUpdate(float deltaTime)
{
	lifespan -= deltaTime;
	if (lifespan <= 0.f)
	{
		// Delete the shapes and set it to Kinematic
		detachAllShapes();
		setKinematic(true);
	}
}
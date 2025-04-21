#pragma once
#include "Joints.h"
#include "Collider.h"
#include <vector>
#include <map>

#ifndef STRUCTURE_H
#define STRUCTURE_H

using namespace std;

class Foundation : public StaticActor
{
public:
	Foundation(const PxTransform& pose = PxTransform(PxIdentity), float foundationSize = 1.f);

	float getSize() const { return foundationSize; }
private:
	float foundationSize;
};

class StructureMaterial : public Collider
{
public:
	StructureMaterial(const PxTransform& pose = PxTransform(PxIdentity))
		: Collider(pose) {
	}
	~StructureMaterial() { Break(); }

	void CustomUpdate(float deltaTime)
	{
		if (broken) return;

		// Check if the anchors are still in place
		breakCount = 0;
		for (Actor* anchor : anchors)
			if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(anchor))
				if (material->IsBroken())
					breakCount++;

		// If breakCount is greater than the threshold
		if (breakCount > anchors.size() / 4)
			Break();
	}

	void Break()
	{
		if (broken) return;

		// Remove the Joints from other StructureMaterials
		for (auto it = joints.begin(); it != joints.end(); )
		{
			pair<Joint* const, Actor*>& entry = *it;
			Joint* joint = entry.first;
			Actor* actor = entry.second;

			if (StructureMaterial* material = dynamic_cast<StructureMaterial*>(actor))
			{
				material->RemoveJoint(joint);
				material->BreakAnchor(this);
			}

			++it;
		}

		// Break each Joint
		for (auto it = joints.begin(); it != joints.end(); )
		{
			pair<Joint* const, Actor*>& entry = *it;
			Joint* joint = entry.first;
			Actor* actor = entry.second;

			delete joint;
			it = joints.erase(it);
		}

		broken = true;

		CustomBreak();
	}
	virtual void CustomBreak() = 0;

	bool IsBroken() const { return broken; }
	float getBreakForce() const { return breakForce; }

	void AddJoint(Joint* joint, Actor* other) { joints[joint] = other; }

	void RemoveJoint(Joint* joint)
	{
		auto it = joints.find(joint);
		if (it != joints.end())
			joints.erase(it);
	}

	void BreakAnchor(StructureMaterial* anchor)
	{
		// Remove the joints which connect to the anchor
		for (auto it = joints.begin(); it != joints.end(); )
		{
			pair<Joint* const, Actor*>& entry = *it;
			Joint* joint = entry.first;
			Actor* actor = entry.second;
			if (actor == anchor)
			{
				delete joint;
				it = joints.erase(it);
			}
			else
				++it;
		}

	}

protected:
	void AddAnchor(Actor* anchor) { anchors.push_back(anchor); }

	vector<Actor*> anchors;
	map<Joint*, Actor*> joints;

	bool broken = false;
	int breakCount = 0;

	float breakForce = 0.f;
};

class SupportPillar : public StructureMaterial
{
public:
	SupportPillar(Foundation* foundation, Actor* ancorActor, int floor, float floorHeight);

	void OnCollision(Collider* other);
	void CustomBreak() {}
};

class StructureFloor : public StructureMaterial
{
public:
	StructureFloor(const PxTransform& pose = PxTransform(PxIdentity), float width = 1.f, float depth = 1.f, float height = 0.1f, int floor = 0, float floorHeight = 1.f, vector<Foundation*> foundations = vector<Foundation*>(), vector<SupportPillar*> supportPillars = vector<SupportPillar*>());

	void OnCollision(Collider* other);
	void CustomBreak() {}
};

class Window : public StructureMaterial
{
public:
	Window(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 size = PxVec3(0.f, 0.f, 0.f), vector<PxVec3> jointLocations = vector<PxVec3>(), vector<PxVec3> attachment0JointLocations = vector<PxVec3>(), vector<PxVec3> attachment1JointLocations = vector<PxVec3>(), Actor* attachment0 = nullptr, Actor* attachment1 = nullptr);

	void OnCollision(Collider* other);
	void CustomBreak();

	PxVec3 getSize() const { return size; }

	bool IsShattered() const { return shattered; }
	void Shatter() { shattered = true; }

	PxTransform getBreakTransform() const { return breakTransform; }
	PxVec3 getBreakVelocity() const { return breakVelocity; }
	PxVec3 getBreakAngularVelocity() const { return breakAngularVelocity; }

private:
	PxVec3 size;
	bool shattered = false;

	PxTransform breakTransform = PxTransform(PxIdentity);
	PxVec3 breakVelocity = PxVec3(0.f, 0.f, 0.f);
	PxVec3 breakAngularVelocity = PxVec3(0.f, 0.f, 0.f);
};

class WindowShard : public DynamicActor
{
public:
	WindowShard(const PxTransform& pose = PxTransform(PxIdentity), const PxVec3& initialVelocity = PxVec3(0.f, 0.f, 0.f), const PxVec3& initialAngularVelocity = PxVec3(0.f, 0.f, 0.f), const PxConvexMeshGeometry& mesh = PxConvexMeshGeometry(), float lifespan = 0.f);

	void CustomUpdate(float deltaTime);

private:
	float lifespan = 0.f;
};

class WindowShards
{
public:
	WindowShards(Window* window);

	void Update(float deltaTime);

	vector<Actor*> getShards();

	bool AddedToScene = false;

private:
	vector<WindowShard*> shards;
	Window* window;
};

class Structure
{
public:
	Structure(const PxTransform& pose = PxTransform(PxIdentity), int floors = 1, int width = 1, int depth = 1);
	~Structure();

	void Update(float deltaTime);

	void Collapse();

	vector<Actor*> getActors();
	vector<Actor*> getMissingActors();

	int getScore();
	int getMaxScore();

private:
	vector<Foundation*> foundations;
	vector<SupportPillar*> supportPillars;
	vector<StructureFloor*> floors;
	vector<Window*> windows;
	vector<WindowShards*> windowShardsCollection;
};

#endif // !STRUCTURE_H;



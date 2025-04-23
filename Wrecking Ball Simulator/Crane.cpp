#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"
#include "Helpful.h"

#include <PxPhysicsAPI.h>
#include <vector>

using namespace physx;

// --- Wheel Implementation ---
Wheel::Wheel(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
    createShape(PxBoxGeometry(size/2, size/1, size/1), 0.0f);
	setColour(Helpful::RGBtoScalar(0.0f, 0.0f, 0.0f)); // Black color
}

Wheel::~Wheel() {}

// --- CraneBase Implementation ---
CraneBottom::CraneBottom(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
	PxTransform basePose = pose * PxTransform(PxVec3(0, size / 2, 0));
    createShape(PxBoxGeometry(size*2.2, size / 3, size*2.2), 0.0f);
    setColour(Helpful::RGBtoScalar(100.0f, 100.0f, 100.0f)); // Grey color
    setPosition(basePose.p);

	// Spawn wheels
    PxTransform wheelPoses[4] = {
    pose * PxTransform(PxVec3(2 * size, 0, 2 * size)),
    pose * PxTransform(PxVec3(-2 * size, 0, 2 * size)),
    pose * PxTransform(PxVec3(2 * size, 0, -2 * size)),
    pose * PxTransform(PxVec3(-2 * size, 0, -2 * size))
    };

    for (int i = 0; i < 4; ++i) {
        wheels[i] = new Wheel(wheelPoses[i], size / 2);
        printf("Wheel spawned at position: %f, %f, %f\n", wheelPoses[i].p.x, wheelPoses[i].p.y, wheelPoses[i].p.z);
    }
}

CraneBottom::~CraneBottom() {}

std::vector<Actor*> CraneBottom::getActors() {
	std::vector<Actor*> actors;
	for (int i = 0; i < 4; ++i) {
		actors.push_back(wheels[i]);
	}
	actors.push_back(this);
	return actors;
}

// --- CraneCore Implementation ---
CraneTop::CraneTop(const PxTransform& pose, float size, float length) : StaticActor(pose), size(size), length(length) {
    PxTransform corePose = pose * PxTransform(PxVec3(0, size*1.8f, 0));
    createShape(PxBoxGeometry(size * 2.2, size, size * 2.2), 0.0f);
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
    setPosition(corePose.p);
}

CraneTop::~CraneTop() {}

std::vector<Actor*> CraneTop::getActors() {
	std::vector<Actor*> actors;
	actors.push_back(this);
	return actors;
}

// --- Crane Implementation ---
Crane::Crane(const PxTransform& pose, float Size, float Length) 
    : DynamicActor(pose), Size(Size), Length(Length){

    //Center of the crane : pose
	PxTransform bottomPose = pose * PxTransform(PxVec3(0, Size / 2, 0));
	PxTransform topPose = pose * PxTransform(PxVec3(0, Size * 1.8f + Length, 0));

	bottom = new CraneBottom(bottomPose, Size);
	top = new CraneTop(topPose, Size, Length);
	
	thePoint = new DynamicActor(pose);
	thePoint->createShape(PxSphereGeometry(.5f), 1.0f);
}

Crane::~Crane() {
    delete bottom;
    delete top;
}

void Crane::Move(PxVec2 movementOffset) {
	for (Actor* actor : getActors()) {
		PxVec3 currentPosition = actor->getPosition();
		PxVec3 newPosition = currentPosition + PxVec3(movementOffset.x, 0, movementOffset.y);
		actor->setPosition(newPosition);
	}
}

vector<Actor*> Crane::getActors() {
    vector<Actor*> actors;  
    // Add actors from wheels  
    vector<Actor*>topActors = top->getActors();  
	actors.insert(actors.end(), topActors.begin(), topActors.end());
	vector<Actor*> bottomActors = bottom->getActors();
	actors.insert(actors.end(), bottomActors.begin(), bottomActors.end());

	actors.push_back(thePoint);

    return actors;  
}

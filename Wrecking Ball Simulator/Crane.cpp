#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"
#include "Helpful.h"

#include <PxPhysicsAPI.h>
#include <vector>

using namespace physx;

// --- Wheel Implementation ---
Wheel::Wheel(const PxTransform& pose, float size) : DynamicActor(pose), size(size) {
    createShape(PxBoxGeometry(size/2, size/1, size/1), 1.0f);
	setColour(Helpful::RGBtoScalar(0.0f, 0.0f, 0.0f)); // Black color
}

Wheel::~Wheel() {}

// --- CraneBase Implementation ---
CraneBottom::CraneBottom(const PxTransform& pose, float size) : DynamicActor(pose), size(size) {
    float width = size * 2.2f;
    float height = size / 2.5f;
    float depth = size * 2.2f;

    PxTransform basePose = pose * PxTransform(PxVec3(0, size / 2, 0));
    createShape(PxBoxGeometry(width, height, depth), 1.0f);
    setColour(Helpful::RGBtoScalar(100.0f, 100.0f, 100.0f)); // Grey color
    setPosition(basePose.p);

    // Spawn wheels
    PxTransform wheelPoses[4] = {
        pose * PxTransform(PxVec3(2 * size, 0, 2 * size)),
        pose * PxTransform(PxVec3(-2 * size, 0, 2 * size)),
        pose * PxTransform(PxVec3(2 * size, 0, -2 * size)),
        pose * PxTransform(PxVec3(-2 * size, 0, -2 * size))
    };

    // Joint points
    PxTransform jointPoses[4] = {
        PxTransform(PxVec3(width, -height, depth)),
        PxTransform(PxVec3(-width, -height / 2, depth)),
        PxTransform(PxVec3(width, -height / 2, -depth)),
        PxTransform(PxVec3(-width, -height / 2, -depth))
    };

    // Create wheels and joints
    for (int i = 0; i < 4; ++i) {
        wheels[i] = new Wheel(wheelPoses[i], size / 2);
        printf("Wheel spawned at position: %f, %f, %f\n", wheelPoses[i].p.x, wheelPoses[i].p.y, wheelPoses[i].p.z);

        // Create joints between the wheels and the bottom plate
        new RevoluteJoint(wheels[i], PxTransform(PxVec3(0, 0, 0)), this, jointPoses[i]);
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
CraneTop::CraneTop(const PxTransform& pose, float size, float length, CraneBottom* bottom) : DynamicActor(pose), size(size), length(length), bottom(bottom) {
	float width = size * 2.2f;
    float height = size;
    float depth = size * 2.2f;
    PxTransform corePose = pose * PxTransform(PxVec3(0, size*2.8f, 0));
    createShape(PxBoxGeometry(width, height, depth), 1.0f);
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
    setPosition(corePose.p);

    // Create a transform with X axis aligned to world Y axis
    PxQuat rot = PxQuat(PxPi / 2, PxVec3(0, 0, 1)); // 90 deg around Z aligns X to Y
    PxTransform jointFrame(rot);

    // Now use localFrame for both actors if you want them to spin around Y
	RevoluteJoint* joint = new RevoluteJoint(bottom, PxTransform(jointFrame.p + PxVec3(0,height,0), jointFrame.q), this, PxTransform(jointFrame.p + PxVec3(0, -height/2, 0), jointFrame.q));
	joint->setLimit(PxJointAngularLimitPair(-PxPi / 2, PxPi / 2)); // Limit rotation to +/- 90 degrees
	joint->setDriveVelocity(0.0f); // No drive velocity
	joint->setProjectionLinearTolerance(0.1f);
	joint->setProjectionAngularTolerance(0.1f);
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
	top = new CraneTop(topPose, Size, Length, bottom);
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

void Crane::Rotate(Actor* target, float direction)  
{  
   // Use the correct PxQuat constructor that takes an angle in radians and a normalized axis.  
   PxQuat rotation(direction * rotationSpeed, PxVec3(0, 1, 0));  
    PxQuat currentRotation = target->getOrientation();
	PxQuat newRotation = currentRotation * rotation;
    target->setOrientation(newRotation);
}

void Crane::Update(float deltaTime, InputManager* inputManager, Camera* camera) {
    if (inputManager->isKeyPressed(static_cast<unsigned char>('w')))
        Move(PxVec2(0.0f, 1.0f)); // Move forward
    if (inputManager->isKeyPressed(static_cast<unsigned char>('s')))
        Move(PxVec2(0.0f, -1.0f)); // Move backward
    if (inputManager->isKeyPressed(static_cast<unsigned char>('a')))
        Rotate(this, -20.0f); // Rotate left
    if (inputManager->isKeyPressed(static_cast<unsigned char>('d')))
        Rotate(this, 20.0f); // Rotate right
    if (inputManager->isKeyPressed(static_cast<unsigned char>('q')))
        Rotate(top, -10.0f); // Rotate top left
    if (inputManager->isKeyPressed(static_cast<unsigned char>('e')))
        Rotate(top, 10.0f); // Rotate top right
}

vector<Actor*> Crane::getActors() {
    vector<Actor*> actors;  
    // Add actors from wheels  
    vector<Actor*>topActors = top->getActors();  
	actors.insert(actors.end(), topActors.begin(), topActors.end());
	vector<Actor*> bottomActors = bottom->getActors();
	actors.insert(actors.end(), bottomActors.begin(), bottomActors.end());
    return actors;  
}

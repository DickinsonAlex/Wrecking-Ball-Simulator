#include "Crane.h"
#include "PhysicsEngine.h"
#include "Structure.h"
#include "Helpful.h"

#include <PxPhysicsAPI.h>
#include <vector>

using namespace physx;

// --- Wheel Implementation ---
Wheel::Wheel(const PxTransform& pose, float size) : DynamicActor(pose), size(size) {
    createShape(PxSphereGeometry(size), 5.0f);
	setColour(Helpful::RGBtoScalar(0.0f, 0.0f, 0.0f)); // Black color
}

Wheel::~Wheel() {}

// --- CraneBottom Implementation ---
CraneBottom::CraneBottom(const PxTransform& pose, float size) : DynamicActor(pose), size(size) {
    float width = size * 2.2f;
    float height = size / 2.5f;
    float depth = size * 2.2f;

    PxTransform basePose = pose * PxTransform(PxVec3(0, size / 2, 0));
    createShape(PxBoxGeometry(width, height, depth), 5000000000.0f); // Increased density for more mass
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
        PxTransform(PxVec3(width, (-height) - 3, depth - 4)),
        PxTransform(PxVec3(-width, (-height / 2) - 3, depth - 4)),
        PxTransform(PxVec3(width, (-height / 2) - 3, 4 - depth)),
        PxTransform(PxVec3(-width, (-height / 2) - 3, 4 - depth))
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

// --- CraneArmBottom Implementation ---
CraneArmBottom::CraneArmBottom(const PxTransform& pose, float size, float length) : DynamicActor(pose), size(size), length(length) {
	createShape(PxBoxGeometry(length, size, size+1), 100.0f); // Increased density for more mass
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
	setPosition(pose.p);
}

CraneArmBottom::~CraneArmBottom() { delete this; }

// --- CraneArmTop Implementation ---
CraneArmTop::CraneArmTop(const PxTransform& pose, float size, float length) : DynamicActor(pose), size(size), length(length) {
	createShape(PxBoxGeometry(size+.1f, size, length+6), 100.0f); // Increased density for more mass
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
	setPosition(pose.p);
}

CraneArmTop::~CraneArmTop() { delete this; }

// --- CraneTop Implementation ---
CraneTop::CraneTop(const PxTransform& pose, float size, float length, CraneBottom* bottom) : DynamicActor(pose), size(size), length(length), bottom(bottom) {
	float width = size * 2.2f;
    float height = size;
    float depth = size * 2.2f;
    PxTransform corePose = pose * PxTransform(PxVec3(0, size*2.8f, 0));
    createShape(PxBoxGeometry(width, height, depth), 10.0f); // Increased density for more mass
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
    setPosition(corePose.p);

    // Create a transform with X axis aligned to world Y axis
    PxQuat rot = PxQuat(PxPi / 2, PxVec3(0, 0, 1)); // 90 deg around Z aligns X to Y
    PxTransform jointFrame(rot);

    // Now use localFrame for both actors to restrict rotation to Y-axis only
    RevoluteJoint* joint = new RevoluteJoint(
        bottom, 
        PxTransform(jointFrame.p + PxVec3(0, height, 0), jointFrame.q), 
        this, 
        PxTransform(jointFrame.p + PxVec3(0, -height / 2, 0), jointFrame.q)
    );

    // Disable drive velocity to prevent unintended movement
    joint->setDriveVelocity(0.0f); 

    // Tighten tolerances to reduce wobble
    joint->setProjectionLinearTolerance(100.f); 
    joint->setProjectionAngularTolerance(100.f);

    // Create the crane arm
    CraneArmBottom* armBottom = new CraneArmBottom(pose * PxTransform(PxVec3(0, size * 2.8f, size * 2.8f)), size / 2, length*12);

    // Connect them with a joint
    PxTransform armJointPoseLocal = PxTransform(PxVec3(0, size * 2.8f, size * 2.8f), PxQuat(PxPi / 2, PxVec3(0, 0, 1)));
    PxTransform armJointPoseWorld = pose * armJointPoseLocal;

    // Use the correct local and world transforms for the FixedJoint
    new FixedJoint(armBottom, PxTransform(PxVec3(0, 0, 0)), this, armJointPoseLocal);

	// Create the top part of the crane arm
	CraneArmTop* armTop = new CraneArmTop(pose * PxTransform(PxVec3(0, size * 2.8f, size * 2.8f)), size / 2, length * 12);

	// Connect the arm top and bottom with a joint
	PxTransform armTopJointPoseLocal = PxTransform(PxVec3(0, 0, size * 2.8f), PxQuat(PxPi / 2, PxVec3(0, 0, 1)));
	PxTransform armTopJointPoseWorld = pose * armTopJointPoseLocal;
	new FixedJoint(armTop, PxTransform(PxVec3(0, size * 2.8f, 0)), armBottom, armTopJointPoseLocal);
	armTop->setName("Crane Arm Top");
	armBottom->setName("Crane Arm Bottom");

    
    actors.push_back(this); // Add the arm to the list of actors
	actors.push_back(armBottom); // Add the arm to the list of actors
	actors.push_back(armTop); // Add the arm to the list of actors
}

CraneTop::~CraneTop() {}

std::vector<Actor*> CraneTop::getActors() {
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

Actor* Crane::getTop() {
	return top;
}

void Crane::Move(PxVec2 movementOffset) {  
    PxQuat orientation = bottom->getOrientation(); // Get the crane's current orientation  
    PxVec3 forward = orientation.rotate(PxVec3(0, 0, 1)); // Forward direction relative to orientation  
    PxVec3 right = orientation.rotate(PxVec3(1, 0, 0)); // Right direction relative to orientation  

    // Calculate movement in world space based on the offset and orientation  
    PxVec3 movement = forward * movementOffset.y + right * movementOffset.x;  

    for (Actor* actor : getActors()) {  
        PxVec3 currentPosition = actor->getPosition();
        PxVec3 newPosition = currentPosition + movement;  
        actor->setPosition(newPosition);
    }  

    // Update the position of the crane
    PxVec3 currentPosition = getPosition();
    PxVec3 newPosition = currentPosition + movement;
    setPosition(newPosition);
}

void Crane::Rotate(Actor* target, float direction)  
{  
    // Use the correct PxQuat constructor that takes an angle in radians and a normalized axis.  
    PxQuat rotation(direction * rotationSpeed, PxVec3(0, 1, 0));  
    PxQuat currentRotation = target->getOrientation();
    PxQuat newRotation = currentRotation * rotation;
    target->setOrientation(newRotation);
}

void Crane::Update(float deltaTime, InputManager* inputManager) {
    if (inputManager->isKeyPressed(static_cast<unsigned char>('w')))
        Move(PxVec2(0.0f, 0.5f)); // Move forward
    if (inputManager->isKeyPressed(static_cast<unsigned char>('s')))
        Move(PxVec2(0.0f, -0.5f)); // Move backward
    if (inputManager->isKeyPressed(static_cast<unsigned char>('a')))
        Rotate(bottom, -5.0f); // Rotate left
    if (inputManager->isKeyPressed(static_cast<unsigned char>('d')))
        Rotate(bottom, 5.0f); // Rotate right
    if (inputManager->isKeyPressed(static_cast<unsigned char>('q')))
        Rotate(top, 20.0f); // Rotate top left
    if (inputManager->isKeyPressed(static_cast<unsigned char>('e')))
        Rotate(top, -20.0f); // Rotate top right
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

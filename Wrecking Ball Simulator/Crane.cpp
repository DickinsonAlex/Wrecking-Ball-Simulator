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

// --- Wheels Implementation ---  
Wheels::Wheels(const PxTransform& pose, float size) : StaticActor(pose) {  
   PxTransform wheelPoses[4] = {  
       pose * PxTransform(PxVec3(2 * size, 0, 2 * size)),
       pose * PxTransform(PxVec3(-2 * size, 0, 2 * size)),
       pose * PxTransform(PxVec3(2 * size, 0, -2 * size)),
       pose * PxTransform(PxVec3(-2 * size, 0, -2 * size))
   };  

   for (int i = 0; i < 4; ++i) {  
       wheels[i] = new Wheel(wheelPoses[i], size/2);  
       printf("Wheel spawned at position: %f, %f, %f\n", wheelPoses[i].p.x, wheelPoses[i].p.y, wheelPoses[i].p.z);  
   }  
}

std::vector<Actor*> Wheels::getActors() {
	std::vector<Actor*> wheelActors;
	for (int i = 0; i < 4; ++i) {
		wheelActors.push_back(wheels[i]);
	}
	return wheelActors;
}

Wheels::~Wheels() {}

// --- CraneBase Implementation ---
CraneBase::CraneBase(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
	PxTransform basePose = pose * PxTransform(PxVec3(0, size / 2, 0));
    createShape(PxBoxGeometry(size*2.2, size / 3, size*2.2), 0.0f);
    setColour(Helpful::RGBtoScalar(100.0f, 100.0f, 100.0f)); // Grey color
    setPosition(basePose.p);
}

CraneBase::~CraneBase() {}

// --- CraneCore Implementation ---
CraneCore::CraneCore(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
    PxTransform corePose = pose * PxTransform(PxVec3(0, size*1.8f, 0));
    createShape(PxBoxGeometry(size * 2.2, size, size * 2.2), 0.0f);
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
    setPosition(corePose.p);
}

CraneCore::~CraneCore() {}

// --- CraneLowerArm Implementation ---
CraneLowerArm::CraneLowerArm(const PxTransform& pose, float size, float length) : StaticActor(pose), size(size), length(length) {
	// Create the arm shape
	PxTransform armPose = pose * PxTransform(PxVec3(0, size * 1.8f, 0));
	createShape(PxBoxGeometry(1, length*size*50, 1), 0.0f);
	setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
	setPosition(armPose.p);
}

CraneUpperArm::CraneUpperArm(const PxTransform& pose, float size, float length) : StaticActor(pose), size(size), length(length) {
    // Create the arm shape
    PxTransform armPose = pose * PxTransform(PxVec3(0, size * 1.8f, 0));
    createShape(PxBoxGeometry(1, length * size * 50, 1), 0.0f);
    setColour(Helpful::RGBtoScalar(240.f, 255.f, 36.f)); // Yellow color
    setPosition(armPose.p);
}

// --- CraneArm Implementation ---
CraneArm::CraneArm(const PxTransform& pose, float size, float length)
    : StaticActor(pose), size(size), length(length), lowerArm(nullptr), upperArm(nullptr) {
    // Create the upper and lower arm shapes
    PxTransform lowerArmPose = pose * PxTransform(PxVec3(0, size * 1.8f, 0));
    PxTransform upperArmPose = pose * PxTransform(PxVec3(0, size * 1.8f + length, 0));

    lowerArm = new CraneLowerArm(lowerArmPose, size, length);
    upperArm = new CraneUpperArm(upperArmPose, size, length);
}

std::vector<Actor*> CraneArm::getActors() {
	std::vector<Actor*> armActors;
	armActors.push_back(lowerArm);
	armActors.push_back(upperArm);
	return armActors;
}

// --- CraneHook Implementation ---
CraneHook::CraneHook(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
    //createShape(PxBoxGeometry(size / 4, size / 2, size / 4), 10.0f);
    //setPosition(pose.p);
}

CraneHook::~CraneHook() {}

// --- Chain Implementation ---
Chain::Chain(const PxTransform& pose, float size) : StaticActor(pose), size(size) {
    //createShape(PxCapsuleGeometry(size / 8, size), 10.0f);
    //setPosition(pose.p);
}

Chain::~Chain() {}

// --- WreckingBall Implementation ---
WreckingBall::WreckingBall(const PxTransform& pose, float size) : StaticActor(pose), size(size){
    //createShape(PxSphereGeometry(size), 3.0f);
    //setPosition(pose.p);
}

WreckingBall::~WreckingBall() {}

void WreckingBall::onColision(Actor* other) {
    // Handle collision logic here
}

// --- Crane Implementation ---
Crane::Crane(const PxTransform& pose, float Size, float Length) 
    : Size(Size), Length(Length) {

    //Center of the crane : pose
	PxTransform wheelPose = pose * PxTransform(PxVec3(0, -Size / 2, 0));
	PxTransform basePose = pose * PxTransform(PxVec3(0, Size / 2, 0));
	PxTransform corePose = pose * PxTransform(PxVec3(0, Size, 0));
	PxTransform armPose = pose * PxTransform(PxVec3(0, Length, 1.2f));
	PxTransform hookPose = pose * PxTransform(PxVec3(0, Size, 1.2f + Length));
	PxTransform chainPose = pose * PxTransform(PxVec3(0, Size, 1.2f + Length));
	PxTransform ballPose = pose * PxTransform(PxVec3(0, -Size / 2, 1.2f + Length));

    wheels = new Wheels(pose, Size);
    base = new CraneBase(pose, Size);
    core = new CraneCore(pose, Size);
    arm = new CraneArm(pose, Size, Length);
    hook = new CraneHook(pose, Size);
    chain = new Chain(pose, Length);
    ball = new WreckingBall(pose, Size);
}

Crane::~Crane() {
    delete wheels;
    delete base;
    delete core;
    delete arm;
    delete hook;
    delete chain;
    delete ball;
}

void Crane::Update(PxReal deltaTime, InputManager* inputManager, Camera* camera) {  
 PxVec3 movement(0.0f, 0.0f, 0.0f);  

 if (inputManager->isKeyPressed(static_cast<unsigned char>('W'))) {  
     movement.z += 1.0f; // Move forward  
 }  
 if (inputManager->isKeyPressed(static_cast<unsigned char>('S'))) {  
     movement.z -= 1.0f; // Move backward  
 }  
 if (inputManager->isKeyPressed(static_cast<unsigned char>('A'))) {  
     movement.x -= 1.0f; // Move left  
 }  
 if (inputManager->isKeyPressed(static_cast<unsigned char>('D'))) {  
     movement.x += 1.0f; // Move right  
 }  

 // Apply movement to the crane's position  
 PxTransform currentPose = this->getPxActor()->getGlobalPose();  
 currentPose.p += movement * deltaTime * 5.0f; // Adjust speed multiplier as needed  
 this->getPxActor()->setGlobalPose(currentPose);  
}

std::vector<Actor*> Crane::getActors() {
    std::vector<Actor*> actors;  
    // Add actors from wheels  
    std::vector<Actor*> wheelActors = wheels->getActors();  
    actors.insert(actors.end(), wheelActors.begin(), wheelActors.end());  

    // Add other components  
    actors.push_back(base);  
    actors.push_back(core);  
    actors.push_back(arm);  
    actors.push_back(hook);  
    actors.push_back(chain);  
    actors.push_back(ball);  

    return actors;  
}

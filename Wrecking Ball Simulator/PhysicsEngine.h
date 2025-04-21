#pragma once
#include <string>
#include <PxPhysicsAPI.h>
#include "Level.h"
#include "InputManager.h"
#include "Renderer.h"

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

namespace PhysicsEngine 
{
	using namespace std;

	void Start();
	void Init(const char* title, int width, int height);
	void Update();
	void PxInit();
	void PxShutdown();
	void RenderScene();
	void windowReshapeCallback(int width, int height);

	// Getters
	PxPhysics* getPhysics();
	PxCooking* getCooking();
	Scene* getScene();
	PxMaterial* getMaterial(PxU32 index = 0);
	PxReal getUptime();

	// Setters
	void setScene(Scene* newScene);

	// Input handling
	void KeyPress(unsigned char key, int x, int y);
	void KeyRelease(unsigned char key, int x, int y);
	void mouseCallback(int button, int state, int x, int y);
	void mouseMotionCallback(int x, int y);
	void exitCallback();

	// PhysX Stuff
	extern PxVec3 DefaultColour;
	PxMaterial* createMaterial(PxReal staticFriction = .0f, PxReal dynamicFriction = .0f, PxReal coefficientRestitution = .0f);
	PxConvexMeshGeometry CreateConvexMeshGeometry(vector<PxVec3>& verts, float x = 0.f, float y = 0.f, float z = 0.f, PxVec3 rotation = PxVec3(0.f, 0.f, 0.f));
}

#endif // PHYSICSENGINE_H
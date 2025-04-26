#pragma once  
#include <PxPhysicsAPI.h>  
#include <string>  
#include <vector>  
#include <GL/glut.h>  
#include "Actor.h"  
#include "UserData.h"  

#ifndef RENDERER_H  
#define RENDERER_H  

namespace Renderer 
{  
	using namespace std;
	using namespace physx;

	void ResizeWindow(int width, int height);

	void Init(const char* title, int width, int height);

	void Render(PxActor** actors, const PxU32 numActors);
	void Render(const PxRenderBuffer& data, PxReal lineWidth = 1.f);

	void Start(const PxVec3& cameraEye, const PxVec3& cameraDir);
	void End();
}  

#endif // RENDERER_H

#pragma once  
#include <PxPhysicsAPI.h>  
#include <string>  
#include <vector>  
#include <GL/glut.h>  
#include "Actor.h"  
#include "UserData.h"  
#include "FontRenderer.h"  

#ifndef RENDERER_H  
#define RENDERER_H  

namespace Renderer {  
   void Init(const std::string& title, int width, int height);  
   void InitWindow(const char* title, int width, int height);  
   void Shutdown();  
   void ResizeWindow(int width, int height);  
   void RenderScene(Actor** actors);  
   void RenderText(const std::string& text, int x, int y);  
   void RenderBuffer(float* pVertList, float* pColorList, int type, int num);  
   void DrawPlane();  
   void DrawSphere(const physx::PxGeometryHolder& geometry);  
   void DrawBox(const physx::PxGeometryHolder& geometry);  
   void DrawCapsule(const physx::PxGeometryHolder& geometry);  
   void DrawConvexMesh(const physx::PxGeometryHolder& geometry);  
   void DrawTriangleMesh(const physx::PxGeometryHolder& geometry);  
   void RenderGeometry(const physx::PxGeometryHolder& geometry);  
   void RenderCloth(const physx::PxCloth* cloth);  
   void Render(const physx::PxActor** actors, const physx::PxU32 numActors);  
   void Render(const physx::PxRenderBuffer& data, physx::PxReal lineWidth);  
   void Start(const physx::PxVec3& cameraEye, const physx::PxVec3& cameraDir);  
   void Shutdown();  
}  

#endif // RENDERER_H

#pragma once  
#include <PxPhysicsAPI.h>  
#include <string>  
#include <vector>  

#ifndef ACTOR_H  
#define ACTOR_H  

using namespace std;  
using namespace physx;  

class Actor  
{  
protected:  
   int id; // Actor ID  
   PxActor* actor; // Pointer to the actor  
   string name; // Actor name  
   vector<PxVec3> colours; // Shape colors  

public:  
   Actor(); // Constructor  
   virtual ~Actor(); // Destructor  

   // Getters  
   PxActor* getPxActor() { return actor; }; // Get the PxActor pointer  
   string getName(); // Get the actor name  
   int getId() { return id; }; // Get the actor ID  
   PxVec3 getPosition() const; // Get the actor position  
   PxVec3 getOrientation() const; // Get the actor rotation  
   const PxVec3* GetColour(PxU32 shapeIndex); // Get shape color  

   // Setters  
   void setName(const string& name); // Set the actor name  
   void setId(int id); // Set the actor ID  
   void setPosition(const PxVec3& position); // Set the actor position  
   void setRotation(const PxQuat& rotation); // Set the actor rotation  
   void setColour(PxVec3 colour, PxU32 shapeIndex = -1); // Set shape color  
   void setMaterial(PxMaterial* material, PxU32 shapeIndex = -1); // Set shape material  

   // Shape management  
   PxShape* getShape(PxU32 shapeIndex); // Get a specific shape  
   vector<PxShape*> getShapes(PxU32 shapeIndex = -1); // Get all shapes or a specific one  
   void detachShape(PxU32 shapeIndex); // Detach a specific shape  
   void detachAllShapes(); // Detach all shapes  
};  

class DynamicActor : public Actor  
{  
public:  
   DynamicActor(const PxTransform& pose); // Constructor  
   virtual ~DynamicActor(); // Destructor  

   void createShape(const PxGeometry& geometry, PxReal density); // Create a shape with density  
   void setKinematic(bool value, PxU32 index = -1); // Set kinematic state  
};  

class StaticActor : public Actor  
{  
public:  
   StaticActor(const PxTransform& pose); // Constructor  
   virtual ~StaticActor(); // Destructor  

   void createShape(const PxGeometry& geometry, PxReal density = 0); // Create a shape  
};  

#endif // ACTOR_H

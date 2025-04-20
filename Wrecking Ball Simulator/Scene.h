#pragma once  
#include <PxPhysicsAPI.h> // Include PhysX API
#include "Camera.h" // Include Camera class  
#include "Actor.h" // Include Actor class  

#ifndef SCENE_H
#define SCENE_H  

class Scene  
{  
public:  
   void Init(); // Initialize the scene  
   void Update(float deltaTime); // Update the scene  

   // Getters  
   Camera* getCamera() const; // Get the camera  
   Actor* getActor() const; // Get the actor  

   // Setters  
   void setCamera(Camera* camera); // Set the camera  
   void setActor(Actor* actor); // Set the actor  

   void KeyDown(unsigned char key); // Handle key down events
   void setMousePosition(PxVec2); // Set mouse position


private:  
   Camera* camera; // Pointer to the camera  
   Actor* actor; // Pointer to the actor  
};  

#endif // SCENE_H

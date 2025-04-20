#include "Scene.h"

void Scene::Init()  
{  
   // Initialize camera and actor pointers to nullptr  
   camera = nullptr;  
   actor = nullptr;  
}  

void Scene::Update(float deltaTime)  
{  
   // Update the camera if it exists  
   if (camera)  
   {  
       camera->update(deltaTime);  
   }  

   // Additional scene update logic can be added here  
}  

Camera* Scene::getCamera() const  
{  
   return camera;  
}  

Actor* Scene::getActor() const  
{  
   return actor;  
}  

void Scene::setCamera(Camera* camera)  
{  
   this->camera = camera;  
}  

void Scene::setActor(Actor* actor)  
{  
   this->actor = actor;  
}  

void Scene::KeyDown(unsigned char key)  
{  
   // Handle key down events  
   // Example: Move the actor or camera based on key input  
   if (actor)  
   {  
       PxVec3 position = actor->getPosition();  

       switch (key)  
       {  
       case 'w':  
           position.y += 1.0f; // Move up  
           break;  
       case 's':  
           position.y -= 1.0f; // Move down  
           break;  
       case 'a':  
           position.x -= 1.0f; // Move left  
           break;  
       case 'd':  
           position.x += 1.0f; // Move right  
           break;  
       default:  
           break;  
       }  

       actor->setPosition(position);  
   }  
}  

void Scene::setMousePosition(PxVec2 mousePosition)  
{  
	// Currently will do nothing with the mouse position, will later change the camera to orbit the actor in third person and use this function
    return;
}

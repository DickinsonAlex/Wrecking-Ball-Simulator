#include <vector> // Include vector for list of actors  
#include <PxPhysicsAPI.h> // Include PhysX API for PxRigidActor
#include "Camera.h" // Include Camera class
#include "Actor.h" // Include Actor class

class Scene  
{  
public:  
	  void Init(); // Initialize the scene  
	  void Update(float deltaTime); // Update the scene  

	  // Getters  
	  Camera* getCamera(); // Get the camera  
	  std::vector<Actor*> getActors(); // Get the list of actors  
	  std::vector<PxActor*> getPxActors(); // Get the list of PxActors

	  // Setters  
	  void setCamera(Camera* camera); // Set the camera  
	  void addActor(Actor* actor); // Add an actor to the list  
	  void removeActor(Actor* actor); // Remove an actor from the list  

	  void KeyDown(unsigned char key); // Handle key down events  
	  void setMousePosition(PxVec2); // Set mouse position  

private:  
	  Camera* camera; // Pointer to the camera  
	  std::vector<Actor*> actors; // List of actors  
};

#include "Structure.h"
#include "PhysicsEngine.h"
#include "Helpful.h"

brick::brick(const PxTransform& pose, int size) : DynamicActor(pose), size(size)
{
	// Create a box shape for the brick
	createShape(PxBoxGeometry(size / 2.f, size / 4.f, size / 4.f), 10.0f);
	setColour(Helpful::RGBtoScalar(220.f, 0.f, 0.f)); // Red color
	setName("Brick");
	printf("Brick spawned at position: %f, %f, %f\n", pose.p.x, pose.p.y, pose.p.z);
}

// Destructor
brick::~brick()
{
	delete this;
}
wallStructure::wallStructure(const PxTransform& pose, int height, int length, int depth, int blockSize)
	: DynamicActor(pose), height(height), length(length), depth(depth)
{
   actors = std::vector<Actor*>(); // Initialize the vector to store actors
   // Create a wall of boxes in a staggered brick pattern
   for (int i = 0; i < height; ++i)
   {
       for (int j = 0; j < length; ++j)
       {
           for (int k = 0; k < depth; ++k)
           {
               // Calculate the position of each brick
               float offset = (i % 2 == 0) ? 0 : blockSize / 2.0f; // Offset every other row for staggered pattern
               PxVec3 position = pose.p + PxVec3(j * blockSize + offset, i * (blockSize * 0.5f), k * blockSize);
               brick* b = new brick(PxTransform(position), blockSize);
               b->setColour(Helpful::RGBtoScalar(255.f, 0.f, 0.f)); // Red color
               b->setName("Brick");
               actors.push_back(b); // Store the brick in the vector
           }
       }
   }
}

//Deconstructor
wallStructure::~wallStructure()
{
	for (auto actor : actors)
	{
		delete actor; // Delete each brick
	}
	delete this; // Delete the wall structure itself
}

// Get all actors in the wall structure
std::vector<Actor*> wallStructure::getActors() {
	return actors;
}
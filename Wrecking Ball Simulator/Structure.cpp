#include "Structure.h"
#include "PhysicsEngine.h"
#include "Helpful.h"

Structure::Structure(const PxTransform& pose, int floors, int width, int depth)
{
	// Create a box
	createShape(PxBoxGeometry(PxVec3(width, 0.5f, depth)), 1.f);
}

//Deconstructor
Structure::~Structure()
{
	delete this;
}

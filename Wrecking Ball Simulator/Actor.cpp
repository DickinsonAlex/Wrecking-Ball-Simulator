#include "Actor.h"
#include "PhysicsEngine.h"
#include "UserData.h"

// Actor constructor
Actor::Actor()
{
	// Initialize the actor with default values
	pxActor = PhysicsEngine::getPhysics()->createRigidDynamic(PxTransform(PxVec3(0, 0, 0))); // Default transform
	id = 0;
	name = "";
	colours.clear();
}

// Actor destructor
Actor::~Actor()
{
	// Clean up associated user data
	for (unsigned int i = 0; i < colours.size(); i++)
		delete (UserData*)getShape(i)->userData;
}

// From PhysX Tutorials
void Actor::setColour(PxVec3 colour, PxU32 shapeIndex)
{
	if (shapeIndex == -1)
		for (unsigned int i = 0; i < colours.size(); i++)
			colours[i] = colour;
	else if (shapeIndex < colours.size())
		colours[shapeIndex] = colour;
}

void Actor::setId(int id)
{
	this->id = id;
}

// From PhysX Tutorials
const PxVec3* Actor::GetColour(PxU32 shapeIndex)
{
	if (shapeIndex < colours.size())
		return &colours[shapeIndex];
	else
		return 0;
}

// From PhysX Tutorials
void Actor::setName(const string& name)
{
	this->name = name;
	pxActor->setName(name.c_str());
}

// From PhysX Tutorials
string Actor::getName()
{
	return name;
}

// From PhysX Tutorials
void Actor::setMaterial(PxMaterial* material, PxU32 shapeIndex)
{
	vector<PxShape*> shapes = getShapes(shapeIndex);
	for (PxU32 i = 0; i < shapes.size(); i++)
	{
		vector<PxMaterial*> materials(shapes[i]->getNbMaterials());

		for (unsigned int j = 0; j < materials.size(); j++)
			materials[j] = material;

		shapes[i]->setMaterials(materials.data(), (PxU16)materials.size());
	}
}

// From PhysX Tutorials
PxShape* Actor::getShape(PxU32 shapeIndex)
{
	vector<PxShape*> shapes(((PxRigidActor*)pxActor)->getNbShapes());

	if (shapeIndex < ((PxRigidActor*)pxActor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size()))
		return shapes[shapeIndex];
	else
		return 0;
}

vector<PxShape*> Actor::getShapes(PxU32 shapeIndex)
{
	vector<PxShape*> shapes(((PxRigidActor*)pxActor)->getNbShapes());

	if (shapes.size() == 0)
		return shapes;

	((PxRigidActor*)pxActor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

	if (shapeIndex == -1)
		return shapes;
	else if (shapeIndex < shapes.size())
		return vector<PxShape*>(1, shapes[shapeIndex]);
	else
		return vector<PxShape*>();
}

void Actor::detachShape(PxU32 shapeIndex)
{
	vector<PxShape*> shapes = getShapes();

	if (shapeIndex < shapes.size())
	{
		PxShape* shape = shapes[shapeIndex];

		// Clean up associated user data
		if (shape->userData)
		{
			delete static_cast<UserData*>(shape->userData);
			shape->userData = nullptr;
		}

		// Detach shape from actor
		((PxRigidActor*)pxActor)->detachShape(*shape, true);

		// Remove Colour associated with this shape
		colours.erase(colours.begin() + shapeIndex);
	}
}

void Actor::detachAllShapes()
{
	vector<PxShape*> shapes = getShapes();

	for (PxU32 i = 0; i < shapes.size(); i++)
	{
		PxShape* shape = shapes[i];

		// Clean up associated user data
		if (shape->userData)
		{
			delete static_cast<UserData*>(shape->userData);
			shape->userData = nullptr;
		}

		// Detach shape from actor
		((PxRigidActor*)pxActor)->detachShape(*shape, true);
	}

	colours.clear();
}

// Implement getPosition()
PxVec3 Actor::getPosition() const
{
    return ((PxRigidActor*)pxActor)->getGlobalPose().p;
}

// Implement setPosition()
void Actor::setPosition(const PxVec3& position)
{
    PxTransform transform = ((PxRigidActor*)pxActor)->getGlobalPose();
    transform.p = position;
    ((PxRigidActor*)pxActor)->setGlobalPose(transform);
}

// Implement getOrientation()
PxVec3 Actor::getOrientation() const
{
    PxQuat orientation = ((PxRigidActor*)pxActor)->getGlobalPose().q;
    return orientation.getBasisVector0(); // Example: return one of the basis vectors
}

// Implement setRotation()
void Actor::setRotation(const PxQuat& rotation)
{
    PxTransform transform = ((PxRigidActor*)pxActor)->getGlobalPose();
    transform.q = rotation;
    ((PxRigidActor*)pxActor)->setGlobalPose(transform);
}

// From PhysX Tutorials
DynamicActor::DynamicActor(const PxTransform& pose) : Actor()
{
	pxActor = (PxRigidActor*)PhysicsEngine::getPhysics()->createRigidDynamic(pose);
	setName("");
}

// From PhysX Tutorials
DynamicActor::~DynamicActor()
{
	for (unsigned int i = 0; i < colours.size(); i++)
		delete (UserData*)getShape(i)->userData;
}

void DynamicActor::createShape(const PxGeometry& geometry, PxReal density)  
{
   PxMaterial* material = PhysicsEngine::getMaterial();  
   PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eSIMULATION_SHAPE;  
   PxShape* shape = ((PxRigidDynamic*)pxActor)->createShape(geometry, *material, shapeFlags);  

   if (!shape) {  
       std::cerr << "Error: createShape failed. Check pxActor, geometry, and material." << std::endl;  
       //return;  
   }  

   PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)pxActor, density);  
   colours.push_back(PhysicsEngine::DefaultColour);  
   shape->userData = new UserData();  
   for (unsigned int i = 0; i < colours.size(); i++)  
       ((UserData*)getShape(i)->userData)->color = &colours[i];  
}

// From PhysX Tutorials
void DynamicActor::setKinematic(bool value, PxU32 index)
{
	((PxRigidDynamic*)pxActor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, value);
}

// From PhysX Tutorials
StaticActor::StaticActor(const PxTransform& pose)
{
	pxActor = (PxRigidActor*)PhysicsEngine::getPhysics()->createRigidStatic(pose);
	setName("");
}

// From PhysX Tutorials
StaticActor::~StaticActor()
{
	for (unsigned int i = 0; i < colours.size(); i++)
		delete (UserData*)getShape(i)->userData;
}

// From PhysX Tutorials
void StaticActor::createShape(const PxGeometry& geometry, PxReal density)
{
	PxShape* shape = ((PxRigidStatic*)pxActor)->createShape(geometry, *PhysicsEngine::getMaterial());
	colours.push_back(PhysicsEngine::DefaultColour);
	//pass the Colour pointers to the renderer
	shape->userData = new UserData();
	for (unsigned int i = 0; i < colours.size(); i++)
		((UserData*)getShape(i)->userData)->color = &colours[i];
}
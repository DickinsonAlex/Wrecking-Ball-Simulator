#include "Actor.h"
#include "GameManager.h"
#include "UserData.h"

// From PhysX Tutorials
void Actor::setColour(PxVec3 colour, PxU32 shapeIndex)
{
	if (shapeIndex == -1)
		for (unsigned int i = 0; i < colours.size(); i++)
			colours[i] = colour;
	else if (shapeIndex < colours.size())
		colours[shapeIndex] = colour;
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
	actor->setName(name.c_str());
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
	vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());

	if (shapeIndex < ((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size()))
		return shapes[shapeIndex];
	else
		return 0;
}

vector<PxShape*> Actor::getShapes(PxU32 shapeIndex)
{
	vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());

	if (shapes.size() == 0)
		return shapes;

	((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

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
		((PxRigidActor*)actor)->detachShape(*shape, true);

		// Remove color associated with this shape
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
		((PxRigidActor*)actor)->detachShape(*shape, true);
	}

	colours.clear();
}

// From PhysX Tutorials
DynamicActor::DynamicActor(const PxTransform& pose) : Actor()
{
	actor = (PxRigidActor*)GameManager::getPhysics()->createRigidDynamic(pose);
	setName("");
}

// From PhysX Tutorials
DynamicActor::~DynamicActor()
{
	for (unsigned int i = 0; i < colours.size(); i++)
		delete (UserData*)getShape(i)->userData;
}

// From PhysX Tutorials
void DynamicActor::createShape(const PxGeometry& geometry, PxReal density)
{
	PxShape* shape = ((PxRigidDynamic*)actor)->createShape(geometry, *GameManager::getMaterial());
	PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)actor, density);
	colours.push_back(GameManager::DefaultColour);
	//pass the color pointers to the renderer
	shape->userData = new UserData();
	for (unsigned int i = 0; i < colours.size(); i++)
		((UserData*)getShape(i)->userData)->colour = &colours[i];
}

// From PhysX Tutorials
void DynamicActor::setKinematic(bool value, PxU32 index)
{
	((PxRigidDynamic*)actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, value);
}

// From PhysX Tutorials
StaticActor::StaticActor(const PxTransform& pose)
{
	actor = (PxRigidActor*)GameManager::getPhysics()->createRigidStatic(pose);
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
	PxShape* shape = ((PxRigidStatic*)actor)->createShape(geometry, *GameManager::getMaterial());
	colours.push_back(GameManager::DefaultColour);
	//pass the color pointers to the renderer
	shape->userData = new UserData();
	for (unsigned int i = 0; i < colours.size(); i++)
		((UserData*)getShape(i)->userData)->colour = &colours[i];
}
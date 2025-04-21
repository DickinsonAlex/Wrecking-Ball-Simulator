#include "Primatives.h"
#include "PhysicsEngine.h"

// mesh cooking (preparation)
PxConvexMesh* ConvexMesh::CookMesh(const PxConvexMeshDesc& mesh_desc)
{
	PxDefaultMemoryOutputStream stream;

	if (!PhysicsEngine::getCooking()->cookConvexMesh(mesh_desc, stream))
		throw new Exception("ConvexMesh::CookMesh, cooking failed.");

	PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

	return PhysicsEngine::getPhysics()->createConvexMesh(input);
}

//mesh cooking (preparation)
PxTriangleMesh* TriangleMesh::CookMesh(const PxTriangleMeshDesc& mesh_desc)
{
	PxDefaultMemoryOutputStream stream;

	if (!PhysicsEngine::getCooking()->cookTriangleMesh(mesh_desc, stream))
		throw new Exception("TriangleMesh::CookMesh, cooking failed.");

	PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

	return PhysicsEngine::getPhysics()->createTriangleMesh(input);
}
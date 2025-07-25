#pragma once
#include "Actor.h"
#include "Exception.h"

// PhyX Tutorial Code
class Plane : public StaticActor
{
public:
	Plane(PxVec3 normal = PxVec3(0.f, 1.f, 0.f), PxReal distance = 0.f)
		: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
	{
		createShape(PxPlaneGeometry());
	}
};

// PhyX Tutorial Code
class Sphere : public DynamicActor
{
public:
	Sphere(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 1.f, PxReal density = 1.f)
		: DynamicActor(pose)
	{
		createShape(PxSphereGeometry(radius), density);
	}
};

// PhyX Tutorial Code
class Box : public DynamicActor
{
public:
	Box(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
		: DynamicActor(pose)
	{
		createShape(PxBoxGeometry(dimensions), density);
	}
};

// PhyX Tutorial Code
class Capsule : public DynamicActor
{
public:
	Capsule(const PxTransform& pose = PxTransform(PxIdentity), PxVec2 dimensions = PxVec2(1.f, 1.f), PxReal density = 1.f)
		: DynamicActor(pose)
	{
		createShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
	}
};

// PhyX Tutorial Code
class ConvexMesh : public DynamicActor
{
public:
	ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f)
		: DynamicActor(pose)
	{
		PxConvexMeshDesc mesh_desc;
		mesh_desc.points.count = (PxU32)verts.size();
		mesh_desc.points.stride = sizeof(PxVec3);
		mesh_desc.points.data = &verts.front();
		mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		mesh_desc.vertexLimit = 256;

		createShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
	}

	PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc);
};

///The TriangleMesh class
class TriangleMesh : public StaticActor
{
public:
	//constructor
	TriangleMesh(const std::vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose = PxTransform(PxIdentity))
		: StaticActor(pose)
	{
		PxTriangleMeshDesc mesh_desc;
		mesh_desc.points.count = (PxU32)verts.size();
		mesh_desc.points.stride = sizeof(PxVec3);
		mesh_desc.points.data = &verts.front();
		mesh_desc.triangles.count = (PxU32)trigs.size() / 3;
		mesh_desc.triangles.stride = 3 * sizeof(PxU32);
		mesh_desc.triangles.data = &trigs.front();

		createShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
	}

	//mesh cooking (preparation)
	PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc);
};
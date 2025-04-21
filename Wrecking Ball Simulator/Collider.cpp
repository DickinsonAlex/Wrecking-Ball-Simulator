#include "Collider.h"
#include "PhysicsEngine.h"
#include "Scene.h"
#include <PxPhysicsAPI.h>

void Collider::Update(float deltaTime)
{
	Scene* scene = PhysicsEngine::getScene();

	if (!scene)
		return;

	const PxU32 numShapes = getPxActor()->getNbShapes();
	PxShape** shapes = new PxShape * [numShapes];
	PxU32 shapeCount = getPxActor()->getShapes(shapes, numShapes);

	const PxU32 maxHits = 256;
	PxOverlapHit hits[maxHits];
	PxOverlapBuffer hitBuffer(hits, maxHits);

	for (PxU32 i = 0; i < shapeCount; i++)
	{
		PxShape* shape = shapes[i];
		if (!shape)
			continue;

		PxGeometryHolder geometry = shape->getGeometry();

		PxTransform shapeTransform = getPxActor()->getGlobalPose() * shape->getLocalPose();

		if (scene->getScene()->overlap(geometry.any(), shapeTransform, hitBuffer))
			for (PxU32 j = 0; j < hitBuffer.nbTouches; j++)
			{
				PxRigidActor* overlappingPxActor = hitBuffer.touches[j].actor;
				if (!overlappingPxActor || overlappingPxActor == getPxActor())
					continue;

				Actor* overlappingActor = scene->getActorFromPxActor(overlappingPxActor);
				if (Collider* collider = dynamic_cast<Collider*>(overlappingActor))
					collider->OnCollision(this);
			}
	}

	delete[] shapes;

	CustomUpdate(deltaTime);
}
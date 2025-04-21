#pragma once
#include "Actor.h"

#ifndef COLLIDER_H
#define COLLIDER_H

class Collider : public DynamicActor
{
public:
	Collider(const PxTransform& pose = PxTransform(PxIdentity))
		: DynamicActor(pose) {
	}

	void Update(float deltaTime);
	virtual void CustomUpdate(float deltaTime) = 0;

	virtual void OnCollision(Collider* other) = 0;
};

#endif // !COLLIDER_H
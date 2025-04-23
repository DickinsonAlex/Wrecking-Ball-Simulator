#pragma once
#include "Joints.h"
#include "Collider.h"
#include <vector>
#include <map>

#ifndef STRUCTURE_H
#define STRUCTURE_H

using namespace std;

class Structure : public Collider
{
public:
	Structure(const PxTransform& pose = PxTransform(PxIdentity), int floors = 1, int width = 1, int depth = 1);
	~Structure();
};

#endif // !STRUCTURE_H;
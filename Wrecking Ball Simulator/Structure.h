#pragma once
#include "Joints.h"
#include <vector>
#include <map>

#ifndef WALLSTRUCTURE_H
#define WALLSTRUCTURE_H

using namespace std;

class brick : public DynamicActor
{
public:
	brick(const PxTransform& pose = PxTransform(PxIdentity), int size = 1);
	~brick();
private:
	int size;
};

class wallStructure : public DynamicActor
{
public:
	wallStructure(const PxTransform& pose = PxTransform(PxIdentity), int height = 1, int length = 1, int depth = 1, int blockSize = 1);
	~wallStructure();
	vector<Actor*> getActors();
	void Update(float deltaTime); // Add update function declaration
private:
	int height;
	int length;
	int depth;
	vector<Actor*> actors; // Store the actors in a vector
};

#endif // !WALLSTRUCTURE_H;
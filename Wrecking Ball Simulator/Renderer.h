#pragma once
#include <PxPhysicsAPI.h>
#include "Actor.h"

#ifndef RENDERER_H
#define RENDERER_H

using namespace physx;
using namespace std;

class Renderer
{
public:
	void Init(); // Initialize the renderer
	void Shutdown(); // Shutdown the renderer

	void RezizeWindow(int width, int height); // Resize the window
	void RenderScene(PxActor** actors); // Render the scene
	void RenderText(const string& text, int x, int y); // Render text
};

#endif // RENDERER_H



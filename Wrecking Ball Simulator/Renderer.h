#pragma once
#include <PxPhysicsAPI.h>
#include <string>
#include "Actor.h"

#ifndef RENDERER_H
#define RENDERER_H

namespace Renderer {
    void Init(const std::string& title, int width, int height);  
	void InitWindow(const char* title, int width, int height);
    void Shutdown();
    void ResizeWindow(int width, int height);
    void RenderScene(Actor** actors);
    void RenderText(const std::string& text, int x, int y);
	void RenderBuffer(float* pVertList, float* pColorList, int type, int num);
	void DrawPlane();
    void Start();
	void Finish();
}

#endif // RENDERER_H

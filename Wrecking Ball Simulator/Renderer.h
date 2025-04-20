#pragma once
#include <PxPhysicsAPI.h>
#include <string>
#include "Actor.h"

namespace Renderer {
    void Init(std::string name, int width, int height);
    void Shutdown();
    void ResizeWindow(int width, int height);
    void RenderScene(Actor** actors);
    void RenderText(const std::string& text, int x, int y);
}

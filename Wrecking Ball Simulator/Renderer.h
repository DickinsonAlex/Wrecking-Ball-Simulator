// Renderer.h
#pragma once
#include "SceneBase.h"

namespace Renderer
{
    void InitWindow(const char* title, int width, int height);
    void Init();
    void RunLoop(SceneBase* scene);
}

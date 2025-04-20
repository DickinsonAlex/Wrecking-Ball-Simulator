#pragma once
#include <string>
#include "Scene.h"
#include "InputManager.h"
#include "Renderer.h"
#include "Actor.h"
#include "Camera.h"

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

namespace GameManager {
    void Start();
    void Init(std::string title, int width, int height);
    void Update();
    void PxInit();
    void PxShutdown();
    PxMaterial* GetMaterial(PxU32 index);
    PxMaterial* CreateMaterial(PxReal staticFriction, PxReal dynamicFriction, PxReal coefficientRestitution);
    void setScene(Scene* newScene);
    void KeyPress(unsigned char key, int x, int y);
    void KeyRelease(unsigned char key, int x, int y);
    void mouseCallback(int button, int state, int x, int y);
    void mouseMotionCallback(int x, int y);
    void exitCallback();
}

#endif // GAMEMANAGER_H
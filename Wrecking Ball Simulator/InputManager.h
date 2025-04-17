// InputManager.h
#pragma once
#include <PxPhysicsAPI.h>
#include <GL/glut.h>
#include <unordered_map>

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void WindowReshape(int w, int h);

    void KeyDown(unsigned char key, int x, int y);
    void KeyUp(unsigned char key, int x, int y);

    void SpecialKeyDown(int key, int x, int y);
    void SpecialKeyUp(int key, int x, int y);

    void CheckModifiers(int modifiers);

    void Mouse(int button, int state, int x, int y);
    physx::PxVec2 MouseMotion(int x, int y) const;

    bool IsKeyDown(unsigned char key) const;
    bool IsSpecialKeyDown(int key) const { return gKeys.at(key); }

    bool IsShiftDown() const { return shift; }
    bool IsCtrlDown() const { return ctrl; }
    bool IsAltDown() const { return alt; }
    bool IsFocused() const { return focused; }

private:
    std::unordered_map<int, bool> gKeys;
    int centerX = 640, centerY = 360;
    bool shift = false, ctrl = false, alt = false;
    bool focused = true;
};

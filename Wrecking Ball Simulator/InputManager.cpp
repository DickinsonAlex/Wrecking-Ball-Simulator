// InputManager.cpp
#include "InputManager.h"

InputManager::InputManager()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
}

InputManager::~InputManager() {}

void InputManager::WindowReshape(int w, int h)
{
    centerX = w / 2;
    centerY = h / 2;
}

void InputManager::KeyDown(unsigned char key, int x, int y)
{
    if (key >= 'A' && key <= 'Z') key += 32;
    if (key == 27) // Escape key
    {
        focused = false;
        glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
    }
    gKeys[key] = true;
}

void InputManager::KeyUp(unsigned char key, int x, int y)
{
    if (key >= 'A' && key <= 'Z') key += 32;
    gKeys[key] = false;
}

void InputManager::SpecialKeyDown(int key, int x, int y)
{
    gKeys[key] = true;
}

void InputManager::SpecialKeyUp(int key, int x, int y)
{
    gKeys[key] = false;
}

void InputManager::CheckModifiers(int modifiers)
{
    shift = modifiers & GLUT_ACTIVE_SHIFT;
    ctrl = modifiers & GLUT_ACTIVE_CTRL;
    alt = modifiers & GLUT_ACTIVE_ALT;
}

void InputManager::Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        focused = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(centerX, centerY);
    }
}

physx::PxVec2 InputManager::MouseMotion(int x, int y) const
{
    int deltaX = centerX - x;
    int deltaY = centerY - y;
    if (focused) glutWarpPointer(centerX, centerY);
    return physx::PxVec2(deltaX, deltaY);
}

bool InputManager::IsKeyDown(unsigned char key) const
{
    if (key >= 'A' && key <= 'Z') key += 32;
    auto it = gKeys.find(key);
    return it != gKeys.end() && it->second;
}
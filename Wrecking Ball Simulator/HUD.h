#pragma once
#include <string>
#include <vector>
#include <PxPhysicsAPI.h>

class HUD
{
public:
    void AddLine(const std::string& text);
    void EditLine(size_t index, const std::string& newText);
    void Render();
    void Clear();

private:
    std::vector<std::string> lines;
    physx::PxVec3 colour = physx::PxVec3(0.f);
    float fontSize = 0.02f;
};

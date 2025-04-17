#include "HUD.h"
#include "FontRenderer.h"

void HUD::AddLine(const std::string& text)
{
    lines.push_back(text);
}

void HUD::EditLine(size_t index, const std::string& newText)
{
    if (index < lines.size())
        lines[index] = newText;
}

void HUD::Render()
{
    float y = 0.95f;
    for (const std::string& line : lines)
    {
        FontRenderer::setColor(colour.x, colour.y, colour.z, 1.f);
        FontRenderer::print(0.02f, y, fontSize, line.c_str());
        y -= fontSize * 1.5f;
    }
}

void HUD::Clear()
{
    lines.clear();
}

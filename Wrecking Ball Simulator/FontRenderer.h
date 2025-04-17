// FontRenderer.h
#pragma once

namespace FontRenderer
{
    void setScreenResolution(int width, int height);
    void setColor(float r, float g, float b, float a);
    void print(float x, float y, float fontSize, const char* text);
}
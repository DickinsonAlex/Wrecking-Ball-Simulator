#include "FontRenderer.h"
#include <GL/glut.h>
#include <cstring>

namespace FontRenderer
{
    static float textColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static int screenWidth = 0;
    static int screenHeight = 0;
    static bool initialized = false;

    void setScreenResolution(int width, int height)
    {
        screenWidth = width;
        screenHeight = height;
        initialized = true;
    }

    void setColor(float r, float g, float b, float a)
    {
        textColor[0] = r;
        textColor[1] = g;
        textColor[2] = b;
        textColor[3] = a;
    }

    void print(float x, float y, float fontSize, const char* text)
    {
        if (!initialized || !text) {
            return; // Silent return if not initialized or text is null
        }

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, screenWidth, 0, screenHeight);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glColor4f(textColor[0], textColor[1], textColor[2], textColor[3]);
        glRasterPos2f(x, y);
        
        // Scale factor based on fontSize (default GLUT font is roughly 8x13)
        float scaleFactor = fontSize / 13.0f;
        
        // Render each character
        for (const char* c = text; *c; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}

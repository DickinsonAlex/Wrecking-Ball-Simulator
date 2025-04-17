// Renderer.cpp
#include "Renderer.h"
#include <GL/glut.h>

namespace Renderer
{
    static SceneBase* gScene = nullptr;

    void DisplayCallback()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (gScene)
        {
            gScene->CustomUpdate(1.0f / 60.0f, nullptr);
        }
        glutSwapBuffers();
    }

    void RunLoop(SceneBase* scene)
    {
        gScene = scene;
        glutDisplayFunc(DisplayCallback);
        glutIdleFunc(DisplayCallback);
        glutMainLoop();
    }

    void InitWindow(const char* title, int width, int height)
    {
        int argc = 1;
        char* argv[1] = { const_cast<char*>(title) };
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(width, height);
        glutCreateWindow(title);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.6f, 0.7f, 0.9f, 1.0f);
    }

    void Init() {}
}
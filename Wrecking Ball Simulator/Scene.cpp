#include "Scene.h"
#include "Camera.h"
#include "Actor.h"
#include <PxPhysicsAPI.h>

using namespace std; 

void Scene::Init()  
{  
    camera = nullptr;  
    actors.clear();  
}  

void Scene::Update(float deltaTime)  
{  
    if (camera)  
    {  
        camera->update(deltaTime);  
    }  

    for (Actor* actor : actors)  
    {  
        if (actor)  
        {  
            // Add actor-specific update logic here if needed  
        }  
    }  
}  

Camera* Scene::getCamera()  
{  
    return camera;  
}  

vector<Actor*> Scene::getActors()  
{  
    return actors;  
}  

vector<PxActor*> Scene::getPxActors()  
{  
    std::vector<PxActor*> pxActors;  
    for (Actor* actor : actors)  
    {  
        if (actor)  
        {  
            PxActor* pxActor = actor->getPxActor();  
            if (pxActor)  
            {  
                pxActors.push_back(pxActor);  
            }  
        }  
    }  
    return pxActors;  
}  

void Scene::setCamera(Camera* cam)  
{  
    camera = cam;  
}  

void Scene::addActor(Actor* actor)  
{  
    if (actor)  
    {  
        actors.push_back(actor);  
    }  
}  

void Scene::removeActor(Actor* actor)  
{  
    actors.erase(std::remove(actors.begin(), actors.end(), actor), actors.end());  
}  

void Scene::KeyDown(unsigned char key)  
{  
    if (!actors.empty())  
    {  
        Actor* actor = actors[0];  
        PxVec3 position = actor->getPosition();  

        switch (key)  
        {  
        case 'w':  
            position.y += 1.0f;  
            break;  
        case 's':  
            position.y -= 1.0f;  
            break;  
        case 'a':  
            position.x -= 1.0f;  
            break;  
        case 'd':  
            position.x += 1.0f;  
            break;  
        default:  
            break;  
        }  

        actor->setPosition(position);  
    }  
}  

void Scene::setMousePosition(PxVec2 mousePosition)  
{  
    // Currently will do nothing with the mouse position, will later change the camera to orbit the actor in third person and use this function  
    return;  
}

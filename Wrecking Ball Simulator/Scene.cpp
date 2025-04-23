#include "Scene.h"
#include "PhysicsEngine.h"
#include "Exception.h"
#include "UserData.h"

#include <GL/glut.h>
#include <PxPhysicsAPI.h>

using namespace std; 

void Scene::Init(Camera* cam, InputManager* iM)
{  
    camera = cam;
	inputManager = iM;

    PxSceneDesc sceneDesc(PhysicsEngine::getPhysics()->getTolerancesScale());
    sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;

    if (!sceneDesc.cpuDispatcher)
    {
        PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(6);
        sceneDesc.cpuDispatcher = mCpuDispatcher;
    }

    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    pxScene = PhysicsEngine::getPhysics()->createScene(sceneDesc);

    if (!pxScene)
        throw new Exception("Could not Initialise Scene.");

    pxScene->setGravity(PxVec3(0.f, -9.81f, 0.f));

    spawnFeatures();
    
    paused = false;
}  

void Scene::Update(float deltaTime, InputManager* inputManager, Camera* camera)
{  
    if (camera)  
    {  
        camera->update(deltaTime);  
    }  
    updateFeatures(deltaTime, inputManager, camera);
    pxScene->simulate(deltaTime);
    pxScene->fetchResults(true);
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

void Scene::addActors(vector<Actor*> actorList)
{
	for (Actor* actor : actorList)
	{
		if (actor)
		{
			actors.push_back(actor);
		}
	}
}

void Scene::removeActor(Actor* actor)  
{  
    actors.erase(std::remove(actors.begin(), actors.end(), actor), actors.end());  
}  

void Scene::setMousePosition(PxVec2 mousePosition)  
{  
    // Currently will do nothing with the mouse position, will later change the camera to orbit the actor in third person and use this function  
    return;  
}

Actor* Scene::getActorFromPxActor(PxActor* actor)
{
	vector<Actor*> actors = getActors();

    for (unsigned int i = 0; i < actors.size(); i++)
        if (actors[i]->getPxActor() == actor)
            return actors[i];

    return 0;
}
#include "PhysicsEngine.h"
#include "Exception.h"
#include "PxPhysicsAPI.h"

#include <GL/glut.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

using namespace std::this_thread;
using namespace physx;
using namespace chrono;

namespace PhysicsEngine {
    Scene* scene = nullptr;
    InputManager* inputManager = nullptr;
    Camera* camera = nullptr;

	// PhysX variables
    PxDefaultErrorCallback defaultErrorCallback;
    PxDefaultAllocator defaultAllocator;
    PxFoundation* foundation = 0;
    PxPvd* pvd = 0;
    PxPhysics* physics = 0;
    PxCooking* cooking = 0;
    PxReal deltaTime = 1.f / 60.f;
	PxReal Uptime = 0.f;
    PxVec3 DefaultColour = PxVec3(0.5f, 0.5f, 0.5f);

    void Start() {
        glutMainLoop();
    }

    void Init(const char* title, int width, int height) {
        PxInit();

		// Initialize the window
        Renderer::Init(title, width, height);


		// Create the input manager
        inputManager = new InputManager();
        glutKeyboardFunc(KeyPress);
        glutKeyboardUpFunc(KeyRelease);
        atexit(exitCallback);

		// Create the scene
        scene = new Level();
        scene->Init(camera, inputManager);

		// Initialize the camera
        camera = new Camera();
        scene->setCamera(camera);

		// Set the scene in the physics engine
        glutDisplayFunc(DrawScene);
        glutReshapeFunc(windowReshapeCallback);
    }

    void Update() {
        if (scene) {
            scene->Update(deltaTime, inputManager, camera);
        }
    }

    PxPhysics* getPhysics() { return physics; }
    PxCooking* getCooking() { return cooking; }
	Scene* getScene() { return scene; }

	// From PhysX Tutorials
    void PxInit() {
        if (!foundation) {
            foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, defaultAllocator, defaultErrorCallback);
            if (!foundation)
                throw new Exception("Could not create PhysX foundation.");
        }

        if (!pvd) {
            pvd = PxCreatePvd(*foundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
            pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        if (!physics) {
            physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);
            if (!physics)
                throw new Exception("Could not initialise PhysX SDK.");
            PxInitExtensions(*physics, pvd);
        }

        if (!cooking) {
            cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
            if (!cooking)
                throw new Exception("Could not initialise the cooking library.");
        }

        createMaterial(0.5f, 0.5f, 0.6f);
    }

	// From PhysX Tutorials
    void PxShutdown() {
        if (cooking)
            cooking->release();
        if (physics)
            physics->release();
        if (pvd)
            pvd->release();
        if (foundation)
            foundation->release();
    }

	// From PhysX Tutorials
    PxMaterial* getMaterial(PxU32 index) {
        std::vector<PxMaterial*> materials(physics->getNbMaterials());
        if (index < physics->getMaterials((PxMaterial**)&materials.front(), (PxU32)materials.size()))
            return materials[index];
        else
            return nullptr;
    }

	// From PhysX Tutorials
    PxMaterial* createMaterial(PxReal staticFriction, PxReal dynamicFriction, PxReal coefficientRestitution) {
        return physics->createMaterial(staticFriction, dynamicFriction, coefficientRestitution);
    }

    void setScene(Scene* newScene) {
        if (scene) {
            delete scene;
        }
        scene = newScene;
    }

    void DrawScene()
    {
        scene->Update(deltaTime, inputManager, camera);
        Renderer::Start(camera->getPosition(), camera->getDirection());
        vector<PxActor*> actors = scene->getPxActors();

        if (!actors.empty())
        {
            Renderer::Render(&actors[0], (PxU32)actors.size());
        }
		Renderer::End();
    }

    void windowReshapeCallback(int width, int height)
    {
        inputManager->reshapeWindow(width, height);
        Renderer::ResizeWindow(width, height);
    }

    void KeyPress(unsigned char key, int x, int y) {
        inputManager->setKeyPressed(key);
    }

    void KeyRelease(unsigned char key, int x, int y) {
        inputManager->setKeyReleased(key);
    }

    void exitCallback() {
        delete scene;
        delete camera;
        PxShutdown();
    }
}

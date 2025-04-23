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
        glutMouseFunc(mouseCallback);
        glutMotionFunc(mouseMotionCallback);
        glutPassiveMotionFunc(mouseMotionCallback);
        atexit(exitCallback);
        mouseMotionCallback(width / 2, height / 2);

		// Create the scene
        scene = new Level();
        scene->Init(camera, inputManager);

		// Initialize the camera
        camera = new Camera(scene->getTarget());
        scene->setCamera(camera);

		// Set the scene in the physics engine
        glutDisplayFunc(RenderScene);
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

    PxConvexMeshGeometry CreateConvexMeshGeometry(vector<PxVec3>& verts, float x, float y, float z, PxVec3 rotation)
    {
        vector<PxVec3> newVerts(verts);

        // Translate the verts in accordance to the rotation (euler angles)
        for (int i = 0; i < newVerts.size(); i++)
        {
            PxVec3 vert = newVerts[i];
            newVerts[i].x = vert.x * cos(rotation.y) - vert.z * sin(rotation.y);
            newVerts[i].z = vert.x * sin(rotation.y) + vert.z * cos(rotation.y);
        }

        // Translate the newVerts to the correct position
        for (int i = 0; i < newVerts.size(); i++)
            newVerts[i] = newVerts[i] + PxVec3(x, y, z);

        PxConvexMeshDesc convexDesc;
        convexDesc.points.count = (PxU32)newVerts.size();
        convexDesc.points.stride = sizeof(PxVec3);
        convexDesc.points.data = newVerts.data();
        convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

        PxDefaultMemoryOutputStream buf;
        PxConvexMeshCookingResult::Enum result;
        if (!PhysicsEngine::getCooking()->cookConvexMesh(convexDesc, buf, &result))
            return PxConvexMeshGeometry();

        PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        PxConvexMesh* convexMesh = PhysicsEngine::getPhysics()->createConvexMesh(input);

        return PxConvexMeshGeometry(convexMesh);
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

    void RenderScene()
    {
        Uptime += deltaTime;
        scene->Update(deltaTime, inputManager, camera);

        auto startPhysics = chrono::high_resolution_clock::now();
        auto endPhysics = high_resolution_clock::now();
        auto physicsTime = duration_cast<milliseconds>(endPhysics - startPhysics).count();
        auto startTime = high_resolution_clock::now();

        Renderer::Start(camera->getPosition(), camera->getDirection());

        vector<PxActor*> actors = scene->getPxActors();
		printf("Actors: %d\n", (int)actors.size());

        if (!actors.empty())
        {
            Renderer::Render(&actors[0], (PxU32)actors.size());
        }

        auto endRender = high_resolution_clock::now();
        auto renderTime = duration_cast<milliseconds>(endRender - startTime).count();

        //printf("Physics: %lldms, Render: %lldms\n", physicsTime, renderTime);

		Renderer::End();

        // sleep to prevent 100% CPU usage
        sleep_for(deltaTime * 1000ms);
    }

    void windowReshapeCallback(int width, int height)
    {
        inputManager->WindowReshape(width, height);
        Renderer::ResizeWindow(width, height);
    }

    void KeyPress(unsigned char key, int x, int y) {
        inputManager->setKeyPressed(key);
    }

    void KeyRelease(unsigned char key, int x, int y) {
        inputManager->setKeyReleased(key);
    }

    void mouseCallback(int button, int state, int x, int y) {
        inputManager->setMouse(button, state, x, y);
    }

    void mouseMotionCallback(int x, int y) {
        scene->setMousePosition(inputManager->getMousePosition());
    }

    void exitCallback() {
        delete scene;
        delete camera;
        PxShutdown();
    }

    PxReal getUptime() {
		return Uptime;
	}
}

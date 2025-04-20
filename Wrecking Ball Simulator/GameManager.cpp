#include "GameManager.h"
#include "Exception.h"
#include "PxPhysicsAPI.h"
#include <GL/glut.h>
#include <chrono>
#include <vector>

using namespace physx;
using namespace chrono;

namespace GameManager {
    Scene* scene = nullptr;
    InputManager* inputManager = nullptr;
    Actor* player = nullptr;
    Camera* camera = nullptr;

    PxDefaultErrorCallback defaultErrorCallback;
    PxDefaultAllocator defaultAllocator;
    PxFoundation* foundation = 0;
    PxPvd* pvd = 0;
    PxPhysics* physics = 0;
    PxCooking* cooking = 0;
    PxReal deltaTime = 1.f / 60.f;

    void Start() {
        glutMainLoop();
    }

    void Init(std::string title, int width, int height) {
        PxInit();
        Renderer::Init(title, width, height);

        scene = new Scene();
        scene->Init();

        player = new Actor();
        player->setName("Player");
        player->setPosition(PxVec3(0, 0, 0));
        player->setRotation(PxQuat(PxIdentity));
        scene->setActor(player);

        camera = new Camera();
        scene->setCamera(camera);

        inputManager = new InputManager();

        glutKeyboardFunc(KeyPress);
        glutKeyboardUpFunc(KeyRelease);
        glutMouseFunc(mouseCallback);
        glutMotionFunc(mouseMotionCallback);
        glutPassiveMotionFunc(mouseMotionCallback);

        atexit(exitCallback);
        mouseMotionCallback(width / 2, height / 2);
    }

    void Update() {
        if (scene) {
            scene->Update(deltaTime);
        }
    }

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

        CreateMaterial(0.5f, 0.5f, 0.6f);
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
    PxMaterial* GetMaterial(PxU32 index) {
        std::vector<PxMaterial*> materials(physics->getNbMaterials());
        if (index < physics->getMaterials((PxMaterial**)&materials.front(), (PxU32)materials.size()))
            return materials[index];
        else
            return nullptr;
    }

	// From PhysX Tutorials
    PxMaterial* CreateMaterial(PxReal staticFriction, PxReal dynamicFriction, PxReal coefficientRestitution) {
        return physics->createMaterial(staticFriction, dynamicFriction, coefficientRestitution);
    }

    void setScene(Scene* newScene) {
        if (scene) {
            delete scene;
        }
        scene = newScene;
    }

    void KeyPress(unsigned char key, int x, int y) {
        inputManager->setKeyPressed(key);
        scene->KeyDown(key);
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
}

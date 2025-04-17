#include "Engine.h"
#include "Exception.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FontRenderer.h"
#include <iostream>

using namespace std;
using namespace physx;

namespace Engine
{
    Camera* camera;
    Scene* scene;
    PxReal deltaTime = 1.f / 60.f;
    float Uptime = 0.f;
    InputManager* inputManager;

    // Physics
    PxDefaultErrorCallback defaultErrorCallback;
    PxDefaultAllocator defaultAllocator;

    PxMaterial* defaultMaterial;
    PxFoundation* foundation = 0;
    PxPvd* pvd = 0;
    PxPhysics* physics = 0;
    PxCooking* cooking = 0;
    PxVec3 DefaultColour = PxVec3(0.2f, 0.2f, 0.2f);


    void Init(const char* title, int width, int height)
    {
        // Initialize Renderer first so OpenGL context is created
        Renderer::InitWindow(title, width, height);
        Renderer::Init();
        
        // Now FontRenderer can be safely initialized
        FontRenderer::setScreenResolution(width, height);
        
        // Initialize PhysX components
		PxInit();

        scene = new Scene();
        scene->Init(new Camera(PxVec3(0, 20, -40), PxVec3(0, -0.3f, 1)));
    }

    // From PhysX Tutorials
    void PxInit()
    {
        if (!foundation)
        {
            foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, defaultAllocator, defaultErrorCallback);

            if (!foundation)
                throw new Exception("Could not create PhysX foundation.");
        }

        if (!pvd)
        {
            pvd = PxCreatePvd(*foundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
            pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        }

        if (!physics)
        {
            physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale(), true, pvd);

            if (!physics)
                throw new Exception("Could not initialise PhysX SDK.");

            PxInitExtensions(*physics, pvd);
        }

        if (!cooking)
        {
            cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));

            if (!cooking)
                throw new Exception("Could not initialise the cooking library.");
        }

        CreateMaterial();
    }

    // From PhysX Tutorials
    void PxRelease()
    {
        if (cooking)
            cooking->release();
        if (physics)
            physics->release();
        if (pvd)
            pvd->release();
        if (foundation)
            foundation->release();
    }

    void Start()
    {
        Renderer::RunLoop(scene);
    }

    PxPhysics* GetPhysics() { return physics; }
    PxCooking* GetCooking() { return cooking; }
    Scene* GetScene() { return scene; }

    float GetUptime() { return 0.f; } // stub for now

    PxMaterial* GetMaterial(PxU32 index) { return defaultMaterial; }

    PxMaterial* CreateMaterial(PxReal staticFriction, PxReal dynamicFriction, PxReal restitution)
    {
        return physics->createMaterial(staticFriction, dynamicFriction, restitution);
    }

    PxConvexMeshGeometry CreateConvexMeshGeometry(std::vector<PxVec3>& verts, float x, float y, float z, PxVec3 rotation)
    {
        // Simple passthrough for now
        PxConvexMeshDesc convexDesc;
        convexDesc.points.count = static_cast<PxU32>(verts.size());
        convexDesc.points.stride = sizeof(PxVec3);
        convexDesc.points.data = verts.data();
        convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

        PxDefaultMemoryOutputStream buf;
        PxConvexMeshCookingResult::Enum result;
        cooking->cookConvexMesh(convexDesc, buf, &result);

        PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        PxConvexMesh* convexMesh = physics->createConvexMesh(input);

        return PxConvexMeshGeometry(convexMesh);
    }
}

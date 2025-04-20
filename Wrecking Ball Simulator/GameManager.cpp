#include "GameManager.h"
#include "Exception.h"
#include "PxPhysicsAPI.h" // Include PhysX API for PxPhysics and PxCooking

#include <chrono>
#include <vector>

using namespace physx;
using namespace chrono;

namespace GameManager
{
	Scene* scene = nullptr; // Pointer to the current scene
	InputManager* inputManager = nullptr; // Pointer to the input manager
	Renderer* renderer = nullptr; // Pointer to the renderer
	Actor* player = nullptr; // Pointer to the player actor
	Camera* camera = nullptr; // Pointer to the camera

	// Physics
	PxDefaultErrorCallback defaultErrorCallback;
	PxDefaultAllocator defaultAllocator;
	PxFoundation* foundation = 0;
	PxPvd* pvd = 0;
	PxPhysics* physics = 0;
	PxCooking* cooking = 0;

	void Start() {
		glutMainLoop()
	}

	void Init() {
		// Initialize the scene
		PxInit();
		
		scene = new Scene();
		scene->Init();

		player = new Actor();
		player->setName("Player");
		player->setPosition(PxVec3(0, 0, 0));
		player->setRotation(PxQuat(PxIdentity));
		scene->setActor(player);

		camera = new Camera();
		scene->setCamera(camera);

		// Initialize the input manager
		inputManager = new InputManager();

		glutKeyboardFunc(KeyPress);
		glutKeyboardUpFunc(KeyRelease);
		glutSpecialFunc(KeySpecial);
		glutSpecialUpFunc(KeySpecialRelease);
		glutMouseFunc(mouseCallback);
		glutMotionFunc(mouseMotionCallback);
		glutPassiveMotionFunc(mouseMotionCallback);

		atexit(exitCallback);
		mouseMotionCallback(width / 2, height / 2);

	}

	void Update() {
		// Update the scene
		if (scene) {
			scene->Update();
		}
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

		// Create a default material
        CreateMaterial(0.5f, 0.5f, 0.6f);
	}

	// From PhysX Tutorials
	void PxShutdown()
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

	// From PhysX Tutorials
	PxMaterial* GetMaterial(PxU32 index)
	{
		vector<PxMaterial*> materials(physics->getNbMaterials());
		if (index < physics->getMaterials((PxMaterial**)&materials.front(), (PxU32)materials.size()))
			return materials[index];
		else
			return 0;
	}

	// From PhysX Tutorials
	PxMaterial* CreateMaterial(PxReal staticFriction, PxReal dynamicFriction, PxReal coefficientRestitution)
	{
		return physics->createMaterial(staticFriction, dynamicFriction, coefficientRestitution);
	}

	void setScene(Scene* newScene) {
		// Set the current scene
		if (scene) {
			delete scene;
		}
		scene = newScene;
	}
}
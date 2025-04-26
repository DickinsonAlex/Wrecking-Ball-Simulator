#include "InputManager.h"
#include <unordered_map>

// Constructor
InputManager::InputManager() {}

// Destructor
InputManager::~InputManager() {}

// Internal state for key and mouse tracking
static std::unordered_map<int, bool> keyStates;
static PxVec2 mousePosition(0.0f, 0.0f);

static int width = 0;
static int height = 0;

// Initialize the input manager
void InputManager::Init(int x, int y) {
	width = x;
	height = y;
    keyStates.clear();
    mousePosition = PxVec2(0.0f, 0.0f);
}

// Update the input manager (placeholder for future logic)
void InputManager::Update() {
    // Currently no specific update logic
}

// Window reshape
void InputManager::reshapeWindow(int x, int y) {
	// Update the viewport and projection matrix if needed
	width = x;
	height = y;
}

// Check if a key is pressed
bool InputManager::isKeyPressed(unsigned char key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

// Set a key as pressed
void InputManager::setKeyPressed(unsigned char key) {
    keyStates[key] = true;
}

// Set a key as released
void InputManager::setKeyReleased(unsigned char key) {
    keyStates[key] = false;
}
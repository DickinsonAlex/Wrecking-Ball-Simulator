#include "InputManager.h"
#include <unordered_map>

// Constructor
InputManager::InputManager() {}

// Destructor
InputManager::~InputManager() {}

// Internal state for key and mouse tracking
static std::unordered_map<int, bool> keyStates;
static PxVec2 mousePosition(0.0f, 0.0f);

// Initialize the input manager
void InputManager::Init() {
    keyStates.clear();
    mousePosition = PxVec2(0.0f, 0.0f);
}

// Update the input manager (placeholder for future logic)
void InputManager::Update() {
    // Currently no specific update logic
}

// Check if a key is pressed
bool InputManager::isKeyPressed(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

// Get the current mouse position
PxVec2 InputManager::getMousePosition() const {
    return mousePosition;
}

// Set a key as pressed
void InputManager::setKeyPressed(int key) {
    keyStates[key] = true;
}

// Set a key as released
void InputManager::setKeyReleased(int key) {
    keyStates[key] = false;
}

// Set mouse button state and position
void InputManager::setMouse(int button, int state, int x, int y) {
    // Update mouse position
    mousePosition = PxVec2(static_cast<float>(x), static_cast<float>(y));
    // Mouse button state can be handled here if needed
}
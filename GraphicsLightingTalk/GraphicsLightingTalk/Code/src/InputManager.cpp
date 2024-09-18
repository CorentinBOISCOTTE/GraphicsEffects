#include "InputManager.h"

std::vector<int> InputManager::keyboardButtons(1024, 0);
std::vector<int> InputManager::mouseButtons(32, 0);
Vector2D InputManager::mousePos = Vector2D();

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyboardButtons[key] = action;
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	mouseButtons[button] = action;
}

void InputManager::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos.x = static_cast<float>(xpos);
	mousePos.y = static_cast<float>(ypos);
}

bool InputManager::IsKeyPressed(int key)
{
	return keyboardButtons[key] == GLFW_PRESS;
}

bool InputManager::IsKeyDown(int key)
{
	return keyboardButtons[key] == GLFW_REPEAT;
}

bool InputManager::isKeyReleased(int key)
{
	return keyboardButtons[key] == GLFW_RELEASE;
}

bool InputManager::GetKey(int key)
{
	return (keyboardButtons[key] == GLFW_PRESS) || (keyboardButtons[key] == GLFW_REPEAT);
}

bool InputManager::IsMouseButtonPressed(int button)
{
	return mouseButtons[button] == GLFW_PRESS;
}

bool InputManager::IsMouseButtonDown(int button)
{
	return mouseButtons[button] == GLFW_REPEAT;
}

bool InputManager::IsMouseButtonReleased(int button)
{
	return mouseButtons[button] == GLFW_RELEASE;
}

bool InputManager::GetButton(int button)
{
	return (mouseButtons[button] == GLFW_PRESS) || (mouseButtons[button] == GLFW_REPEAT);
}

Vector2D InputManager::GetMousePosition()
{
	return mousePos;
}

void InputManager::Destroy()
{
	keyboardButtons.clear();
	mouseButtons.clear();
}

#pragma once
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include "Math/matrice.h"

class InputManager
{
public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	static bool IsKeyPressed(int key);
	static bool IsKeyDown(int key);
	static bool isKeyReleased(int key);
	static bool GetKey(int key);

	static bool IsMouseButtonPressed(int button);
	static bool IsMouseButtonDown(int button);
	static bool IsMouseButtonReleased(int button);
	static bool GetButton(int button);

	static Vector2D GetMousePosition();

	static void Destroy();

private:
	static std::vector<int> keyboardButtons;
	static std::vector<int> mouseButtons;
	static Vector2D mousePos;
};
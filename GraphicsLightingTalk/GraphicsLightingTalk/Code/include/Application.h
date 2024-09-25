#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <cstdint>

#include "Debug.h"
#include "Math/matrice.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "SceneManager.h"
#include "Skybox.h"
#include "Instancing.h"

class Application
{
public:
	Application() = default;
	~Application() = default;
	void Initialize(uint16_t width, uint16_t height);
	void Update();
	void RenderSkybox(Camera camera, Shader* shader, Skybox* skybox);
	ResourceManager resourceManager;
	SceneManager sceneManager;

private:
	GLFWwindow* window;
	void Terminate();
	void CloseWindowInput();
	void PostProcessingInput(Shader* shader);
	void LoadResources();
	Skybox* skybox;
	uint16_t m_width = 0;
	uint16_t m_height = 0;
};

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
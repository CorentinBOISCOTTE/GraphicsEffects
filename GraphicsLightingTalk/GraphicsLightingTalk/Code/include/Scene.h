#pragma once
#include "Graph.h"

class Scene
{
public:
	Scene();
	~Scene() = default;
	Graph<Object*> sceneGraph;
	Camera camera;
	std::vector<Light*> lights;
	Shader* shader;
	std::vector<Material*> materials;
	void Update(GLFWwindow* window, ResourceManager* resourceManager);
	void Draw();
	void Load(GLFWwindow* window, ResourceManager* resourceManager);
	void Unload();
	void (*InitFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
	void (*UpdateFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
};


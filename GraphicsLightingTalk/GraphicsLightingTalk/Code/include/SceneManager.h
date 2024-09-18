#pragma once
#include "Scene.h"

class SceneManager
{
public:
	SceneManager() = default;
	SceneManager(ResourceManager* _resourceManager, GLFWwindow* window);
	~SceneManager();
	std::map<std::string, Scene*> scenes;
	Scene* currentScene;
	void CreateScene(std::string id, void (*InitFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager), void (*UpdateFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager));
	void LoadScene(std::string id);
	void DeleteScene(std::string id);

private:
	Scene* FindScene(std::string id);
	ResourceManager* resourceManager;
	GLFWwindow* window;
};

void SampleScene(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
void UpdateSampleScene(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager);
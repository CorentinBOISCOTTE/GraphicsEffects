#include "Scene.h"

Scene::Scene()
{
	InitFunc = nullptr;
	UpdateFunc = nullptr;
	shader = nullptr;
	sceneGraph.camera = &camera;
}

void Scene::Update(GLFWwindow* window, ResourceManager* resourceManager)
{
	UpdateFunc(this, window, resourceManager);
	sceneGraph.Update(window);
}

void Scene::Draw()
{
	sceneGraph.Draw(lights);
}

void Scene::Load(GLFWwindow* window, ResourceManager* resourceManager)
{
	InitFunc(this, window, resourceManager);
	sceneGraph.shader = shader;
}

void Scene::Unload()
{
	sceneGraph.Clear();
	for (Material* m : materials)
		delete m;
	materials.clear();
	for (Light* l : lights)
		delete l;
	lights.clear();
}

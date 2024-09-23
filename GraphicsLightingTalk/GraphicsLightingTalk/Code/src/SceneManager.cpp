#include "SceneManager.h"

SceneManager::SceneManager(ResourceManager* _resourceManager, GLFWwindow* window)
{
	currentScene = nullptr;
	resourceManager = _resourceManager;
	this->window = window;
}

SceneManager::~SceneManager()
{
	for (std::pair<std::string, Scene*> s : scenes)
	{
		s.second->Unload();
		delete s.second;
	}
	scenes.clear();
}

void SceneManager::CreateScene(std::string id, void (*InitFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager), void (*UpdateFunc)(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager))
{
	Scene* scene = new Scene();
	scene->InitFunc = InitFunc;
	scene->UpdateFunc = UpdateFunc;
	scenes[id] = scene;
}

void SceneManager::LoadScene(std::string id)
{
	Scene* scene = FindScene(id);
	if (!scene)
		return;
	if (currentScene)
		currentScene->Unload();
	currentScene = scene;
	currentScene->Load(window, resourceManager);
}

void SceneManager::DeleteScene(std::string id)
{
	Scene* scene = FindScene(id);
	if (!scene)
		return;
	scene->Unload();
	scenes.erase(id);
}

Scene* SceneManager::FindScene(std::string id)
{
	auto it = scenes.find(id);
	if (it == scenes.end()) {
		std::cout << "No scene with ID " << id << " found" << std::endl;
		return nullptr;
	}
	else
		return it->second;
}

void SampleScene(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	Camera camera;
	camera.Setup(width, height, 45.f, 4.5f, -4.5f, 8.f, -8.f, 0.1f, 10000.f, true, true);
	scene->camera = camera;
	Shader* shader = resourceManager->Get<Shader>("Shader");
	scene->shader = shader;
	Shader* terrainShader = resourceManager->Get<Shader>("TerrainShader");

	//Create Object/Texture
	Model* terrain = resourceManager->Get<Model>("Terrain");
	Texture* grassTexture = resourceManager->Get<Texture>("Grass");
	/*Model* cube = resourceManager->Get<Model>("Cube");
	Texture* earthTexture = resourceManager->Get<Texture>("EarthTexture");
	Model* earth = resourceManager->Get<Model>("Earth");
	//Texture* earthTexture = resourceManager->Get<Texture>("EarthTexture");
	/*Model* moon = resourceManager->Get<Model>("Moon");
	Model* satellite = resourceManager->Get<Model>("Satellite");
	Model* rocket = resourceManager->Get<Model>("Rocket");
	Texture* moonTexture = resourceManager->Get<Texture>("MoonTexture");
	Texture* satelliteTexture = resourceManager->Get<Texture>("SatelliteTexture");
	Texture* rocketTexture = resourceManager->Get<Texture>("RocketTexture");
	Texture* defaultTexture = resourceManager->Get<Texture>("DefaultTexture");*/
	Texture* skyboxTexture = resourceManager->Get<Texture>("Skybox");


	Material* material = new Material(Vector4D(1, 1, 1, 1), Vector4D(1.0f, 1.0f, 1.0f, 1), Vector4D(0.1, 0.1, 0.1, 1));
	scene->materials.push_back(material);

	//Light
	LightingSettings::globalAmbiantColor = Vector4D(0, 0, 0, 1.0f);
	bool enableDirectionalLight = true;
	bool enablePointLight = false;
	bool enableSpotLight = false;

	//Initialise Directional Light
	int directionVisualLightIndex = -1;
	DirectionalLight* directionalLight = nullptr;
	if (enableDirectionalLight)
	{
		directionalLight = new DirectionalLight();
		directionalLight->direction = Vector3D(0, 0.5, 1);
		directionalLight->ambiantColor = Vector4D(0, 0, 0, 1.0f);
		directionalLight->diffuseColor = Vector4D(0.6f, 0.6f, 0.6f, 1.0f);
		directionalLight->specularColor = Vector4D(0.8f, 0.8f, 0.8f, 1.0f);
		directionalLight->intensity = 3.f;
		scene->lights.push_back(directionalLight);
	}

	//Initialise Point Light
		PointLight* pointLight = nullptr;
	if (enablePointLight)
	{
		pointLight = new PointLight();
		pointLight->position = Vector3D(0, 0, 0);
		pointLight->specularColor = Vector4D(0.8f, 0.8f, 0.8f, 1.0f);
		pointLight->linearValue = 0.09f;
		pointLight->intensity = 5000.0f;
		pointLight->quadraticValue = 0.032f;
		scene->lights.push_back(pointLight);

		Vector3D position = Vector3D(0.0f, 0, 0.0f);
		Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	//Initialise Spot Light
	int spotLightVisualIndex0 = -1;
	SpotLight* spotLight = nullptr;
	if (enableSpotLight)
	{
		spotLight = new SpotLight();
		spotLight->position = Vector3D(0, 0, -3);
		spotLight->direction = Vector3D(0, 0, -1);
		spotLight->linearValue = 0.09f;
		spotLight->ambiantColor = Vector4D(0.15f, 0.15f, 0.15f, 1.0f);
		spotLight->spotAngle = 20;
		spotLight->intensity = 2.0f;
		spotLight->spotSmoothValue = 0.2f;
		spotLight->quadraticValue = 0.032f;
		scene->lights.push_back(spotLight);
	}

	scene->sceneGraph.CreateObject("terrain", { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, terrain, grassTexture, material, terrainShader);
	/*scene->sceneGraph.CreateObject("earth", { 0.f, 0.f, -20.f }, { 10.f, 0.f, 0.f }, { 1.f, 1.f, 1.f }, earth, earthTexture, material);
	scene->sceneGraph.CreateObject("earth1", { 0.f, 0.f, -20.f }, { 0.f, 0.f, 0.f }, { 0.5f, 0.5f, 0.5f }, earth, earthTexture, material);
	scene->sceneGraph.CreateObject("moon", { 0.f, 0.f, 30.f }, { 0.f, 0.f, 0.f }, { 0.7f, 0.7f, 0.7f }, moon, moonTexture, material, scene->sceneGraph.FindObject("earth1"));
	scene->sceneGraph.CreateObject("moon1", { 0.f, 0.f, 30.f }, { 0.f, 0.f, 0.f }, { 0.6f, 0.6f, 0.6f }, moon, moonTexture, material, scene->sceneGraph.FindObject("earth1"));
	scene->sceneGraph.CreateObject("satellite", { 0.f, 0.f, 5.f }, { 0.f, 0.f, 0.f }, { 0.001f, 0.001f, 0.001f }, satellite, satelliteTexture, material, scene->sceneGraph.FindObject("earth"));
	scene->sceneGraph.CreateObject("rocket", { 3.f, 0.f, 0.f }, { 90.f, 0.f, 0.f }, { 0.006f, 0.006f, 0.006f }, rocket, rocketTexture, material, scene->sceneGraph.FindObject("moon1"));
	scene->sceneGraph.CreateObject("Skybox", { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, { 100.f, 100.f, 100.f }, cube, Skybox, material);*/
	
}

void UpdateSampleScene(Scene* scene, GLFWwindow* window, ResourceManager* resourceManager)
{
	/*float t = 10.f * MyTime::Get().DeltaTime();
	Object* earth = scene->sceneGraph.FindObject("earth");
	Object* earth1 = scene->sceneGraph.FindObject("earth1");
	Object* moon1 = scene->sceneGraph.FindObject("moon1");
	earth->Rotate({ 0.f, t, 0.f });
	earth1->Rotate({ 0.f, t * 3.f, 0.f});
	moon1->Rotate({ 0.f, 0.f, -t * 7.f });*/
}
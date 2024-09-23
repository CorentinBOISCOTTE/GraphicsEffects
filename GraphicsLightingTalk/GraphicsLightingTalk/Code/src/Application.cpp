#include "Application.h"

void Application::Initialize(uint16_t width, uint16_t height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		return;

	window = glfwCreateWindow(m_width, m_height, "ModernOpenGL_Shadow", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, InputManager::KeyCallback);
	glfwSetMouseButtonCallback(window, InputManager::MouseButtonCallback);
	glfwSetCursorPosCallback(window, InputManager::MousePositionCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
}

void Application::Update()
{
	SceneManager _sceneManager(&resourceManager, window);
	sceneManager = _sceneManager;
	LoadResources();

	sceneManager.CreateScene("SampleScene", SampleScene, UpdateSampleScene);
	sceneManager.LoadScene("SampleScene");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glDepthFunc(GL_LEQUAL);
	
	while (!glfwWindowShouldClose(window))
	{
		MyTime::Get().UpdateDeltaTime();
		CloseWindowInput();

		glClearColor(0.f, 0.f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		RenderSkybox();

		sceneManager.currentScene->camera.Input(MyTime::Get().DeltaTime(), window);
		sceneManager.currentScene->camera.Update(window);

		sceneManager.currentScene->Update(window, &resourceManager);
		sceneManager.currentScene->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	this->Terminate();
}

void Application::RenderSkybox()
{
	glUseProgram(resourceManager.Get<Shader>("SkyboxShader")->GetProgram());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, resourceManager.Get<Texture>("Skybox")->GetID());


	glBindVertexArray(skybox->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glUseProgram(0);
}


void Application::Terminate()
{
	glfwTerminate();
	Log::Get().Destroy();
	MyTime::Get().Destroy();
	InputManager::Destroy();
}

void Application::CloseWindowInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Application::LoadResources()
{
	Shader* shader = resourceManager.Create<Shader>("Shader");
	shader->SetVertexShader("Assets/Shaders/SkyboxVertex.glsl");
	shader->SetFragmentShader("Assets/Shaders/SkyboxFragment.glsl");
	shader->Link();
	Model* cube = resourceManager.Create<Model>("Cube", "Assets/Meshes/cube.obj");
	cube->Load();
	Texture* defaultTexture = resourceManager.Create<Texture>("DefaultTexture", "Assets/Textures/Default.png");
	defaultTexture->Load();
	Model* earth = resourceManager.Create<Model>("Earth", "Assets/Meshes/earth.obj");
	earth->Load();
	/*Texture* earthTexture = resourceManager.Create<Texture>("EarthTexture", "Assets/Textures/earth.png");
	earthTexture->Load();
	Model* moon = resourceManager.Create<Model>("Moon", "Assets/Meshes/moon.obj");
	moon->Load();
	Texture* moonTexture = resourceManager.Create<Texture>("MoonTexture", "Assets/Textures/moon.png");
	moonTexture->Load();
	Model* satellite = resourceManager.Create<Model>("Satellite", "Assets/Meshes/Satellite.obj");
	satellite->Load();
	Texture* satelliteTexture = resourceManager.Create<Texture>("SatelliteTexture", "Assets/Textures/Satellite.jpg");
	satelliteTexture->Load();
	Model* rocket = resourceManager.Create<Model>("Rocket", "Assets/Meshes/Rocket.obj");
	rocket->Load();
	Texture* rocketTexture = resourceManager.Create<Texture>("RocketTexture", "Assets/Textures/Rocket.jpg");
	rocketTexture->Load();
	Texture* Skybox = resourceManager.Create<Texture>("Skybox", "Assets/Textures/Skybox.png");
	Skybox->Load();*/
	Texture* skyboxTexture = resourceManager.Create<Texture>("Skybox");
	skyboxTexture->LoadCubemap({
		"Assets/Textures/Right.png",
		"Assets/Textures/Left.png",
		"Assets/Textures/Top.png",
		"Assets/Textures/Bottom.png",
		"Assets/Textures/Front.png",
		"Assets/Textures/Back.png"
	});
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}

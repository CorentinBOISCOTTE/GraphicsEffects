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
	Shader* shader = resourceManager.Get<Shader>("Shader");
	Skybox* skybox = resourceManager.Get<Skybox>("Skybox");

	sceneManager.CreateScene("SampleScene", SampleScene, UpdateSampleScene);
	sceneManager.LoadScene("SampleScene");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glDepthFunc(GL_LEQUAL);
	
	while (!glfwWindowShouldClose(window))
	{
		MyTime::Get().UpdateDeltaTime();
		CloseWindowInput();

		glClearColor(0.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		RenderSkybox(sceneManager.currentScene->camera, shader, skybox);

		sceneManager.currentScene->camera.Input(MyTime::Get().DeltaTime(), window);
		sceneManager.currentScene->camera.Update(window);

		sceneManager.currentScene->Update(window, &resourceManager);
		sceneManager.currentScene->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	this->Terminate();
}

void Application::RenderSkybox(Camera camera, Shader* shader, Skybox* skybox)
{
	mat4x4 view = camera.getViewMatrix();
	view.GLMCompatible();
	mat4x4 projection = camera.getProjection();
	projection.GLMCompatible();

	skybox->Draw(shader, view, projection);
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
	Skybox* skybox = resourceManager.Create<Skybox>("Skybox");

	skybox->LoadCubemap({
		"Assets/Textures/skybox/right.jpg",
		"Assets/Textures/skybox/left.jpg",
		"Assets/Textures/skybox/top.jpg",
		"Assets/Textures/skybox/bottom.jpg",
		"Assets/Textures/skybox/front.jpg",
		"Assets/Textures/skybox/back.jpg"
		});
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}

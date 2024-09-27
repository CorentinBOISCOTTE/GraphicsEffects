#include "Application.h"

//void SetupDebugCallback();

void Application::Initialize(uint16_t width, uint16_t height)
{
	m_width = width;
	m_height = height;

	if (!glfwInit())
		return;

	window = glfwCreateWindow(m_width, m_height, "GraphicsLightingTalk", NULL, NULL);

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

//#ifndef NDEBUG
//	SetupDebugCallback();
//#endif

	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
}

void Application::Update()
{
	Shader framebufferShader;
	framebufferShader.SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
	framebufferShader.SetFragmentShader("Assets/Shaders/FramebufferFragment.glsl");
	framebufferShader.Link();

	float quadVertices[] = 
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	framebufferShader.UseShader();
	framebufferShader.SetUniformInt("screenTex", 0);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SceneManager _sceneManager(&resourceManager, window);
	sceneManager = _sceneManager;
	LoadResources();
	Shader* shader = resourceManager.Get<Shader>("Shader");
	Skybox* skybox = resourceManager.Get<Skybox>("Skybox");

	sceneManager.CreateScene("SampleScene", SampleScene, UpdateSampleScene);
	sceneManager.LoadScene("SampleScene");

	Shader* instancingShader = resourceManager.Get<Shader>("InstancingShader");
	Texture* treeTexture = resourceManager.Get<Texture>("TreeTexture");
	mat4x4 treesModel = treesModel.Get_TRS_Matrix({ 0, 0, 0 }, { 0, 0, 0 }, { 2.f, 2.f, 2.f });
	Instancing trees("Assets/Meshes/tree.obj", 10000);

	Texture* penguinTexture = resourceManager.Get<Texture>("PenguinTexture");
	mat4x4 penguingModel = penguingModel.Get_TRS_Matrix({ 0, 0, 0 }, { 0, 0, 0 }, { 3.f, 3.f, 3.f });
	Instancing penguins("Assets/Meshes/Penguin.obj", 1000);


	Model* map = resourceManager.Get<Model>("Terrain");
	for (Vertex v : map->vertices)
	{
		if (v.position.y > -11 && v.position.y < 7)
		{
			trees.tempInstances.push_back({ v.position.x, v.position.y, v.position.z, 1.f });
		}
		else if (v.position.y > 23)
		{
			penguins.tempInstances.push_back({ v.position.x, v.position.y, v.position.z, 1.f });
		}
	}
	trees.Load();
	penguins.Load();
	map->vertices.clear();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glDepthFunc(GL_LEQUAL);

	while (!glfwWindowShouldClose(window))
	{
		MyTime::Get().UpdateDeltaTime();
		CloseWindowInput();
		PostProcessingInput(&framebufferShader);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.53f, 0.81f, 0.92f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		RenderSkybox(sceneManager.currentScene->camera, shader, skybox);

		sceneManager.currentScene->camera.Input(MyTime::Get().DeltaTime(), window);
		sceneManager.currentScene->camera.Update(window);

		trees.Draw(sceneManager.currentScene->camera, treesModel, instancingShader, treeTexture);
		penguins.Draw(sceneManager.currentScene->camera, penguingModel, instancingShader, penguinTexture);
		sceneManager.currentScene->Update(window, &resourceManager);
		sceneManager.currentScene->Draw();


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		framebufferShader.UseShader();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &framebuffer);
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
	glfwDestroyWindow(window);
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

void Application::PostProcessingInput(Shader* shader)
{
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/FramebufferFragment.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/Negative.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/GrayScale.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/Sharpen.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/Blur.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/EdgeDetection.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/EdgeDetection2.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		shader->SetVertexShader("Assets/Shaders/FramebufferVertex.glsl");
		shader->SetFragmentShader("Assets/Shaders/PostProcessing/Vignette.glsl");
		shader->Link();
		shader->UseShader();
		shader->SetUniformInt("screenTex", 0);
		shader->SetUniformFloat("radius", 0.6);
		shader->SetUniformFloat("softness", 0.5);
	}
}

void Application::LoadResources()
{
	Shader* shader = resourceManager.Create<Shader>("Shader");
	shader->SetVertexShader("Assets/Shaders/SkyboxVertex.glsl");
	shader->SetFragmentShader("Assets/Shaders/SkyboxFragment.glsl");
	shader->Link();
	Shader* terrainShader = resourceManager.Create<Shader>("TerrainShader");
	terrainShader->SetVertexShader("Assets/Shaders/TerrainVertex.glsl");
	terrainShader->SetFragmentShader("Assets/Shaders/TerrainFragment.glsl");
	terrainShader->Link();
	Shader* reflectionShader = resourceManager.Create<Shader>("ReflectionShader");
	reflectionShader->SetVertexShader("Assets/Shaders/ReflectionVertex.glsl");
	reflectionShader->SetFragmentShader("Assets/Shaders/ReflectionFragment.glsl");
	reflectionShader->Link();
	Terrain terrain;
	terrain.Load("Assets/Textures/iceland_heightmap.png");
	Model* map = resourceManager.Create<Model>("Terrain");
	map->LoadTerrain(terrain);
	Skybox* skybox = resourceManager.Create<Skybox>("Skybox");
	skybox->LoadCubemap({
		"Assets/Textures/skybox/right.png",
		"Assets/Textures/skybox/left.png",
		"Assets/Textures/skybox/top.png",
		"Assets/Textures/skybox/bottom.png",
		"Assets/Textures/skybox/front.png",
		"Assets/Textures/skybox/back.png"
		});
	Model* cube = resourceManager.Create<Model>("Cube", "Assets/Meshes/cube.obj");
	cube->Load();
	Shader* instancingShader = resourceManager.Create<Shader>("InstancingShader");
	instancingShader->SetVertexShader("Assets/Shaders/Instancing/InstancingVertex.glsl");
	instancingShader->SetFragmentShader("Assets/Shaders/Instancing/InstancingFragment.glsl");
	instancingShader->Link();
	Texture* treeTexture = resourceManager.Create<Texture>("TreeTexture", "Assets/Textures/Terrain/grass.jpg");
	treeTexture->Load();
	Model* tree = resourceManager.Create<Model>("Tree", "Assets/Meshes/tree.obj");
	tree->Load();
	Shader* testShader = resourceManager.Create<Shader>("TestShader");
	testShader->SetVertexShader("Assets/Shaders/VertexShader.glsl");
	testShader->SetFragmentShader("Assets/Shaders/FragmentShader.glsl");
	testShader->Link();
	Texture* penguinTexture = resourceManager.Create<Texture>("PenguinTexture", "Assets/Textures/penguin.png");
	penguinTexture->Load();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}


//#ifndef NDEBUG
//void SetupDebugCallback()
//{
//
//	auto callback = [] (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
//	{
//		const char* _source;
//		const char* _type;
//		const char* _severity;
//
//		switch (source) {
//		case GL_DEBUG_SOURCE_API:
//			_source = "API";
//			break;
//
//		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
//			_source = "WINDOW SYSTEM";
//			break;
//
//		case GL_DEBUG_SOURCE_SHADER_COMPILER:
//			_source = "SHADER COMPILER";
//			break;
//
//		case GL_DEBUG_SOURCE_THIRD_PARTY:
//			_source = "THIRD PARTY";
//			break;
//
//		case GL_DEBUG_SOURCE_APPLICATION:
//			_source = "APPLICATION";
//			break;
//
//		case GL_DEBUG_SOURCE_OTHER:
//			_source = "UNKNOWN";
//			break;
//
//		default:
//			_source = "UNKNOWN";
//			break;
//		}
//
//		switch (type) {
//		case GL_DEBUG_TYPE_ERROR:
//			_type = "ERROR";
//			break;
//
//		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
//			_type = "DEPRECATED BEHAVIOR";
//			break;
//
//		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
//			_type = "UDEFINED BEHAVIOR";
//			break;
//
//		case GL_DEBUG_TYPE_PORTABILITY:
//			_type = "PORTABILITY";
//			break;
//
//		case GL_DEBUG_TYPE_PERFORMANCE:
//			_type = "PERFORMANCE";
//			break;
//
//		case GL_DEBUG_TYPE_OTHER:
//			_type = "OTHER";
//			break;
//
//		case GL_DEBUG_TYPE_MARKER:
//			_type = "MARKER";
//			break;
//
//		default:
//			_type = "UNKNOWN";
//			break;
//		}
//
//		switch (severity) {
//		case GL_DEBUG_SEVERITY_HIGH:
//			_severity = "HIGH";
//			break;
//
//		case GL_DEBUG_SEVERITY_MEDIUM:
//			_severity = "MEDIUM";
//			break;
//
//		case GL_DEBUG_SEVERITY_LOW:
//			_severity = "LOW";
//			break;
//
//		case GL_DEBUG_SEVERITY_NOTIFICATION:
//			_severity = "NOTIFICATION";
//			break;
//
//		default:
//			_severity = "UNKNOWN";
//			break;
//		}
//
//		printf("%d: %s of %s severity, raised from %s: %s\n", id, _type, _severity, _source, msg);
//	};
//
//	glEnable(GL_DEBUG_OUTPUT);
//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//	glDebugMessageCallback(callback, nullptr);
//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
//	
//}
//#endif
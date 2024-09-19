#include "Skybox.h" 

float skyboxVertices[] =
{
   -1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
   -1.0f, -1.0f, -1.0f,
   -1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
   -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	//Right
	1, 2, 6,
	6, 5 ,1,
	//Left
	0, 4, 7,
	7, 3, 0,
	//Top
	4, 5 ,6,
	6, 7, 4,
	//Bottom
	0, 3, 2,
	2, 1, 0,
	//Back
	0, 1, 5,
	5, 4, 0,
	//Front
	3, 7, 6,
	6, 2, 3,
};
void Skybox()
{
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string facesCubemap[6] =
	{
		"Assets/Textures/Right.png",
		"Assets/Textures/Left.png",
		"Assets/Textures/Top.png",
		"Assets/Textures/Bottom.png",
		"Assets/Textures/Front.png",
		"Assets/Textures/Back.png"
	};

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
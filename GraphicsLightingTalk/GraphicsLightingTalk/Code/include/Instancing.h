#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "Math/matrice.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include "Buffer.h"
#include "VertexAttributes.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class Instancing
{
public:
	Instancing(std::filesystem::path filename, Shader* shader, Texture* texture, uint32_t nbInstances);
	~Instancing();
	void Draw(Camera camera, mat4x4 model);

private:
	void CreatePositions();
	void BindBuffers();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexBuffer;
	std::vector<Vector4D> instances;
	Shader* shader;
	Texture* texture;
	uint32_t nbInstances;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	VertexAttributes attributes;
};
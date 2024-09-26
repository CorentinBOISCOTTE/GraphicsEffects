#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>
#include <random>
#include <iterator>
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
	Instancing(std::filesystem::path filename, uint32_t nbInstances);
	~Instancing();
	void Load();
	void Draw(Camera camera, mat4x4 model, Shader* shader, Texture* texture);
	uint32_t nbInstances;
	std::vector<Vector4D> tempInstances;

private:
	void BindBuffers();
	void CreatePositions();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexBuffer;
	std::vector<Vector4D> instances;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	GLuint m_IBO = 0;
	std::filesystem::path filename;
};
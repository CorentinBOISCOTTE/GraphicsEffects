#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <vector>
#include "Vertex.h"

class Buffer
{
public:
	Buffer();
	~Buffer();
	void Load();
	void Bind(std::vector<Vertex> vertexBuffer, std::vector<uint32_t> indexBuffer);
	void Reset();

private:
	GLuint m_VBO;
	GLuint m_EBO;
	std::vector<Vertex> m_vertexBuffer;
	std::vector<uint32_t> m_indexBuffer;
};
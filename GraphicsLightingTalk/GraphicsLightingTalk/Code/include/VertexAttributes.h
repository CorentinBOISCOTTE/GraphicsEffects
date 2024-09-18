#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Vertex.h"

class VertexAttributes
{
public:
	VertexAttributes();
	~VertexAttributes();
	void Load();
	void Bind();
	void Setup();
	void Reset();
	void Unload();
	GLuint m_VAO;

private:
};
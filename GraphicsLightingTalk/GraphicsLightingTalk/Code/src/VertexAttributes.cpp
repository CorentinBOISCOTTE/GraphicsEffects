#include "VertexAttributes.h"

VertexAttributes::VertexAttributes()
{
	m_VAO = 0;
}

VertexAttributes::~VertexAttributes()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexAttributes::Load()
{
	glGenVertexArrays(1, &m_VAO);
}

void VertexAttributes::Bind()
{
	glBindVertexArray(m_VAO);
}

void VertexAttributes::Setup()
{
#define POSITION 0
	glVertexAttribPointer(POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(POSITION);
#undef POSITION
#define UV 1
	glVertexAttribPointer(UV, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, textureUV));
	glEnableVertexAttribArray(UV);
#undef UV
#define NORMAL 2
	glVertexAttribPointer(NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(NORMAL);
#undef NORMAL
}

void VertexAttributes::Reset()
{
	glBindVertexArray(0);
}

void VertexAttributes::Unload()
{
	glDeleteVertexArrays(1, &m_VAO);
}

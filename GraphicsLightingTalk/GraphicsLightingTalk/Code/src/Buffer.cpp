#include "Buffer.h"

Buffer::Buffer()
{
	m_VBO = 0;
	m_EBO = 0;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

}

void Buffer::Load()
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

void Buffer::Bind(std::vector<Vertex> vertexBuffer, std::vector<uint32_t> indexBuffer)
{
	m_vertexBuffer = vertexBuffer;
	m_indexBuffer = indexBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertexBuffer.size() * sizeof(Vertex), m_vertexBuffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size() * sizeof(uint32_t), m_indexBuffer.data(), GL_STATIC_DRAW);
}

void Buffer::Reset()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
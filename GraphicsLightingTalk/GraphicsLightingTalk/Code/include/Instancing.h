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

class Instancing
{
public:
	Instancing(std::filesystem::path filename, Shader* shader);
	~Instancing();

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexBuffer;
	Shader* shader;

};
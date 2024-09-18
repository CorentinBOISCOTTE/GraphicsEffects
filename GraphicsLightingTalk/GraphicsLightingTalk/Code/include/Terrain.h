#pragma once
#include <filesystem>

#include <iostream>
#include "VertexAttributes.h"
#include <vector>

class Terrain
{
public:
	Terrain() = default;
	~Terrain() = default;
	void Load(std::filesystem::path fileName);
	void Unload();
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indexBuffer;

private:
	void FillBuffers();
	unsigned char* data;
	int32_t width, height;
	int channels;
};
#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "glad/glad.h"

#include "ResourceManager.h"

class Texture :	public IResource
{
public:
	Texture() = default;
	void Load() override;
	void Unload() override;
	bool IsLoaded() override;
	void Bind();
	void Unbind();
	void LoadCubemap(const std::vector<std::string>& faces);
	GLuint texture;
	GLuint GetID() const { return texture; }

private:
	int width, height;
	int channels; 
	unsigned char* data;
	unsigned char* loadTexture(std::filesystem::path fileame, int& textureWidth, int& textureHeight);
};


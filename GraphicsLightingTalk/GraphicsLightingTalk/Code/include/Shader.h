#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "ResourceManager.h"
#include "Math/matrice.h"

class Shader : public IResource
{
public:
	Shader();
	bool SetVertexShader(std::filesystem::path const& filename);
	bool SetFragmentShader(std::filesystem::path const& filename);
	bool Link();

	void UseShader();
	void SetUniformVector2D(std::string name, Vector2D value);
	void SetUniformVector3D(std::string name, Vector3D value);
	void SetUniformVector4D(std::string name, Vector4D value); 
	void SetUniformMatrix4x4(std::string name, mat4x4 value);
	void SetUniformColor(std::string name, Vector4D value);
	void SetUniformFloat(std::string name, float value);
	void SetUniformInt(std::string name, int value);

	GLuint GetProgram() const { return shaderProgram; }

private:
	void Load() override {}
	void Unload() override {}
	bool IsLoaded() override { return true; }
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
};
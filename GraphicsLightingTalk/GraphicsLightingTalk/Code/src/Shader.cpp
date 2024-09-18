#include "Shader.h"

Shader::Shader()
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
}

bool Shader::SetVertexShader(std::filesystem::path const& filename)
{
	ifstream file(filename);
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char* str = fileContent.c_str();
	glShaderSource(vertexShader, 1, &str, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

bool Shader::SetFragmentShader(std::filesystem::path const& filename)
{
	ifstream file(filename);
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	const char* str = fileContent.c_str();
	glShaderSource(fragmentShader, 1, &str, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

bool Shader::Link()
{
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Shader::UseShader()
{
	glUseProgram(shaderProgram);
}

void Shader::SetUniformVector2D(std::string name, Vector2D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::SetUniformVector3D(std::string name, Vector3D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetUniformVector4D(std::string name, Vector4D value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformColor(std::string name, Vector4D value)
{
	UseShader();
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniformFloat(std::string name, float value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetUniformMatrix4x4(std::string name, mat4x4 value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data);
}

void Shader::SetUniformInt(std::string name, int value)
{
	int location = glGetUniformLocation(shaderProgram, name.c_str());
	glUniform1i(location, value);
}

#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Vertex.h"
#include "Math/matrice.h"
#include "Model.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(Model* _model);
	~Mesh() = default;
	void Update(Camera* camera, mat4x4 TRS);
	void Draw(Camera* camera, Shader* shader, std::vector<Light*> lights, Material* material);
	Model* model;
	mat4x4 modelMatrix;
	Texture* texture;
private:
	mat4x4 mvp;
};
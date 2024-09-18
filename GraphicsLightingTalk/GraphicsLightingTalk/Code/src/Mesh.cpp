#include "Mesh.h"
#include "Camera.h"

Mesh::Mesh(Model* _model)
{
	model = _model;
}

void Mesh::Update(Camera* camera, mat4x4 TRS)
{
	modelMatrix = TRS;
	mvp = camera->getVP() * modelMatrix;
}

void Mesh::Draw(Camera* camera, Shader* shader, std::vector<Light*> lights, Material* material)
{
	model->Draw(camera, shader, lights, mvp, modelMatrix, texture, material);
}

#include "Object.h"

Object::Object(Vector3D position, Vector3D rotation, Vector3D scale, Model* model, Texture* texture, Material* material, Shader* shader, Object* parent)
{
	this->parent = parent;
	this->shader = shader;
	this->model = model;
	this->texture = texture;
	this->model = model;
	this->material = material; 
	mesh.model = this->model;
	mesh.texture = texture;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	localTransform = localTransform.Get_TRS_Matrix(position, rotation, scale);
	if (parent)
	{
		worldTransform = parent->worldTransform * localTransform;
		worldPosition = (parent->worldTransform * Vector4D(position.x, position.y, position.z, 1.0f)).ToVector3D();;
		worldRotation = parent->GetWorldRotation() + rotation;
		worldScale = parent->GetWorldScale() * scale;
	}
	else
	{
		worldTransform = localTransform;
		worldPosition = position;
		worldRotation = rotation;
		worldScale = scale;
	}
}

void Object::Update(Camera* camera)
{
	localTransform = localTransform.Get_TRS_Matrix(position, rotation, scale);
	if (parent)
	{
		worldTransform = parent->worldTransform * localTransform;
		worldPosition = (parent->worldTransform * Vector4D(position.x, position.y, position.z, 1.0f)).ToVector3D();;
		worldRotation = parent->GetWorldRotation() + rotation;
		worldScale = parent->GetWorldScale() * scale;
	}
	else
	{
		worldTransform = localTransform;
		worldPosition = position;
		worldRotation = rotation;
		worldScale = scale;
	}
	
	mesh.Update(camera, worldTransform);
	for (Object* child : children)
		child->Update(camera);
}

void Object::Draw(Camera* camera, std::vector<Light*> lights)
{
	mesh.Draw(camera, shader, lights, material);
	for (Object* child : children)
		child->Draw(camera, lights);
}

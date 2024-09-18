#pragma once
#include <map>
#include <string>
#include "Object.h"

template <typename T>
class Graph
{
public:
	Graph() = default;
	~Graph() = default;
	void CreateObject(std::string id, Vector3D position, Vector3D rotation, Vector3D scale, Model* model, Texture* texture, Material* material, Object* parent = nullptr);
	void RemoveObject(std::string id);
	void SetParent(std::string childID, std::string parentID);
	void RemoveParent(std::string id);
	void Clear();
	Object* FindObject(std::string id);
	bool ObjectExists(std::string id);
	void Update(GLFWwindow* window);
	void Draw(std::vector<Light*> lights);
	std::map<std::string, Object*> objects;
	std::map<std::string, Object*> root;
	std::vector<Light*> lights;
	Camera* camera;
	Shader* shader;
};

template<typename T>
inline void Graph<T>::CreateObject(std::string id, Vector3D position, Vector3D rotation, Vector3D scale, Model* model, Texture* texture, Material* material,  Object* parent)
{
	if (ObjectExists(id))
	{
		std::cout << "Error, already existing object with ID : " << id << std::endl;
		return;
	}
	Object* object = new Object(position, rotation, scale, model, texture, material, parent);
	objects[id] = object;
	if (!parent)
		root[id] = object;
	else
		parent->children.push_back(object);
}

template<typename T>
void Graph<T>::Update(GLFWwindow* window)
{
	camera->Input(MyTime::Get().DeltaTime(), window);
	camera->Update(window);
	for (std::pair<std::string, Object*> o : root)
		o.second->Update(camera);
}

template<typename T>
void Graph<T>::Draw(std::vector<Light*> lights)
{
	for (std::pair<std::string, Object*> o : root)
		o.second->Draw(camera, shader, lights);
}

template<typename T>
void Graph<T>::Clear()
{
	for (std::pair<std::string, Object*> o : objects)
		delete o.second;
	objects.clear();
	root.clear();
	lights.clear();
}

template<typename T>
bool Graph<T>::ObjectExists(std::string id)
{
	auto it = objects.find(id);
	if (it != objects.end())
		return true;
	else
		return false;
}

template<typename T>
void Graph<T>::RemoveObject(std::string id)
{
	Object* object = FindObject(id);
	if (!object)
		return;
	if (object->parent)
		root.erase(id);
	objects.erase(id);
	delete object;
}

template<typename T>
void Graph<T>::SetParent(std::string childID, std::string parentID)
{
	Object* child = FindObject(childID);
	Object* parent = FindObject(parentID);

	if (!child || !parent)
	{
		std::cout << "Object not found" << std::endl;
		return;
	}

	if (!child->parent)
	{
		root.erase(childID);
	}
	else
	{
		auto it = std::find(child->parent->children.begin(), child->parent->children.end(), child);

		if (it != child->parent->children.end())
		{
			int index = it - child->parent->children.begin();
			child->parent->children.erase(child->parent->children.begin() + index);
		}
	}
	mat4x4 invParentWorld = parent->worldTransform.getInverse();
	child->localTransform = child->worldTransform * invParentWorld;
	child->parent = parent;

	Vector3D parentWorldRotationInverse = parent->GetWorldRotation();
	parentWorldRotationInverse = parentWorldRotationInverse.inverseVector();
	Vector3D localPosition = (child->position - parent->GetWorldPosition()) * parentWorldRotationInverse * parent->GetWorldScale();
	Vector3D localRotation = -parent->GetWorldRotation() + child->GetWorldRotation();
	Vector3D localScale = Vector3D(1, 1, 1) / parent->GetWorldScale() * child->GetWorldScale();

	child->SetPosition(localPosition);
	child->SetRotation(localRotation);
	child->SetScale(localScale);

	parent->children.push_back(child);
}

template<typename T>
void Graph<T>::RemoveParent(std::string id)
{
	Object* object = FindObject(id);
	if (!object)
		return;
	if (!object->parent)
	{
		std::cout << "Object with ID : " << id << " already has no parent" << std::endl;
		return;
	}

	object->position = object->worldPosition;
	object->rotation = object->worldRotation;
	object->scale = object->worldScale;
	auto it = std::find(object->parent->children.begin(), object->parent->children.end(), object);
	int index = it - object->parent->children.begin();
	object->parent->children.erase(object->parent->children.begin() + index);

	object->parent = nullptr;
	object->localTransform = object->worldTransform;
	root[id] = object;
}

template<typename T>
Object* Graph<T>::FindObject(std::string id)
{
	auto it = objects.find(id);
	if (it == objects.end()) {
		std::cout << "No object with ID " << id << " found" << std::endl;
		return nullptr;
	}
	else
		return it->second;
}
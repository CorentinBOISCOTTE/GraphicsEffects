#include "Material.h"

Material::Material(Vector4D ambientColor, Vector4D diffuseColor, Vector4D specularColor, float shininess)
{
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor;
	this->shininess = shininess;
}
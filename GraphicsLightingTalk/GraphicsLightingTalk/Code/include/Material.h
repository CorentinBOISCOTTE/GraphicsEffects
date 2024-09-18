#pragma once
#include "Texture.h"
#include "Math/matrice.h"

class Material
{
public:
	Material();
	Material(Vector4D ambientColor, Vector4D diffuseColor, Vector4D specularColor, float shininess = 256);

	Vector4D ambientColor;
	Vector4D diffuseColor;
	Vector4D specularColor;

	float shininess = 256;
};

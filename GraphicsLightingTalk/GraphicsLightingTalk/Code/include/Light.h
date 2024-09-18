#pragma once
#include "Math/matrice.h"

class Light {
public:
	virtual ~Light() = default; 

	enum class LightType
	{
		Directional,
		Point, 
		Spot
	};

	LightType lightType; 
	Vector4D ambiantColor;
	Vector4D diffuseColor; 
	Vector4D specularColor; 
	float intensity;

protected : 
	Light() {
		ambiantColor = { 0.1f, 0.1f, 0.1f, 0.1f };
		diffuseColor = { 1, 1, 1, 1 }; 
		specularColor = { 1, 1, 1, 1 }; 
		intensity = 1.0f; 
		lightType = LightType::Directional; 
	}
};

class DirectionalLight : public Light {
public:
	Vector3D direction;

	DirectionalLight() {
		lightType = LightType::Directional; 
		direction = { 0, 0, 0 }; 
	}
};

class PointLight : public Light {
public:
	Vector3D position;

	float constantValue;
	float linearValue;
	float quadraticValue;

	PointLight() {
		lightType = LightType::Point; 
		constantValue = 1.0f; 
		linearValue = 1.0f; 
		quadraticValue = 1.0f;
	};
};

class SpotLight : public Light {
public:
	Vector3D position; 
	Vector3D direction; 
	float spotAngle; 
	float spotSmoothValue;

	float constantValue; 
	float linearValue; 
	float quadraticValue; 

	SpotLight() {
		lightType = LightType::Spot;
		linearValue = 1.0f;
		constantValue = 1.0f;
		quadraticValue = 1.0f;

		spotAngle = 45;
		spotSmoothValue = 0.1f;
	}
};

class LightingSettings {
public:
	static Vector4D globalAmbiantColor; 
};
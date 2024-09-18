#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Debug.h"

class MyTime
{
public:
	static MyTime& Get();
	void UpdateDeltaTime();
	float DeltaTime() const { return deltaTime; }
	void Destroy();

private:
	MyTime() = default;
	~MyTime() = default;
	inline static MyTime* instance = NULL;
	float deltaTime = 0.f;
	float lastTime = 0.f;
};


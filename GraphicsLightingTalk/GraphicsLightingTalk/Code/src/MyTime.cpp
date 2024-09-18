#include "MyTime.h"

MyTime& MyTime::Get()
{
	if (instance == NULL)
		instance = new MyTime();
	return *instance;
}

void MyTime::UpdateDeltaTime()
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void MyTime::Destroy()
{
	if (instance)
		delete instance;
}

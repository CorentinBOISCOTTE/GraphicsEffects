#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 mvp;
uniform mat4 model;

out vec3 worldPos;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	worldPos = vec3(model * vec4(aPos, 1.0f));
}
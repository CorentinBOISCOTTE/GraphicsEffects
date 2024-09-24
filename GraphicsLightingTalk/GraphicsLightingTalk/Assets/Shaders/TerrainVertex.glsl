#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormalPos;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 normalMVP;

out VS_OUT
{
	vec3 worldPos;
	vec3 normalPos;
} vs_out;

void main()
{
	vs_out.worldPos = vec3(model * vec4(aPos, 1.0f));
	if (vs_out.worldPos.y <= -14)
	{
		gl_Position = vec4(99999999, 999999999, 999999999, 1);
	}
	else
	{
		gl_Position = mvp * vec4(aPos, 1.0f);
	}
	vs_out.normalPos = normalize(mat3(normalMVP) *aNormalPos);
}
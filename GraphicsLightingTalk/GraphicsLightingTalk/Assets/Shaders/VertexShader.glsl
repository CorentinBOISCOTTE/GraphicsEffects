#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aNormalPos;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 normalMVP;

out VS_OUT
{
	vec3 pos;
	vec2 texPos;
	vec3 normalPos;
} vs_out;

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0f);
	vs_out.pos = vec3(model * vec4(aPos, 1.0f));
	vs_out.texPos = aTexPos;
	vs_out.normalPos = normalize(mat3(normalMVP) *aNormalPos);
}
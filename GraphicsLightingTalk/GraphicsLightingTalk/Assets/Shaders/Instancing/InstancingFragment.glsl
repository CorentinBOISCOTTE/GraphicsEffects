#version 460 core

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D text;

void main()
{
	FragColor = texture(text, texPos);
}
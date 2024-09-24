#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTex;

void main()
{
	FragColor = vec4(texture(screenTex, texCoords).rgb, 1.0);
} 
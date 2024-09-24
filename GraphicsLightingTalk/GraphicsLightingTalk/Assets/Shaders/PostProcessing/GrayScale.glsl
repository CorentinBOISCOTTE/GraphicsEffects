#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTex;

void main()
{
	vec4 tempColor = texture(screenTex, texCoords);
	float averageColor = (tempColor.x + tempColor.y + tempColor.z) / 3;
	FragColor = vec4(vec3(averageColor, averageColor, averageColor), 1.0);
} 
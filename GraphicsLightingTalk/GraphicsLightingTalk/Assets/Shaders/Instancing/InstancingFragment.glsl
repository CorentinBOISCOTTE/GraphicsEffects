#version 460

out vec4 FragColor;

in vec2 texPos;

uniform sampler2D text;

void main()
{
	FragcColor = texture(text, texPos);
}
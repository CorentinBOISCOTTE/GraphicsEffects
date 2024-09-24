#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTex;
uniform float radius;
uniform float softness;

void main()
{
	vec4 color = texture2D(screenTex, texCoords);
	float dist = length(texCoords - 0.5);
	float vignette = smoothstep(radius, radius - softness, dist);
	color.rgb = mix(color.rgb, color.rgb * vignette, 1);
	FragColor = color;
} 
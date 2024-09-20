#version 460 core
out vec4 FragColor;

in vec3 worldPos;

void main()
{
	if (worldPos.y > 22)
	{
		FragColor = vec4(1, 1, 1, 1);
	}
	else if (worldPos.y > 8)
	{
		FragColor = vec4(0.78, 0.75, 0.72, 1);
	}
	else if (worldPos.y > -12)
	{
		FragColor = vec4(0.07, 0.49, 0.07, 1);
	}
	else if (worldPos.y > -15)
	{
		FragColor = vec4(1, 0.9, 0.6, 1);
	}
	else
	{
		FragColor = vec4(0, 0, 1, 1);
	}

}
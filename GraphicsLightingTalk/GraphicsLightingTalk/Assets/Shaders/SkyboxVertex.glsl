#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;
    mat4 viewM = mat4(mat3(view));
    gl_Position = (projection * viewM * vec4(aPos, 1.0)).xyzz;
}  
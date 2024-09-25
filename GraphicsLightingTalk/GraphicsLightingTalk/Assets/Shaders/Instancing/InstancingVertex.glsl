#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aNormalPos;
        
out vec2 texPos

uniform mat4 model;
uniform mat4 vp;

layout (binding = 4, std430) buffer Instances
{
   vec4 InstanceData[];
};

void main()
{
    texPos = aTexPos;

    vec3 worldPosition = (model * vec4(InPosition, 1.0)).xyz;

    worldPosition += InstanceData[gl_InstanceID].xyz;

    gl_Position = vp * vec4(worldPosition, 1.0); 
}
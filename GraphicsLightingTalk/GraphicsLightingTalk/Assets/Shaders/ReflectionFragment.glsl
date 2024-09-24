#version 460 core

out vec4 FragColor;

in VS_OUT
{
	vec3 pos;
	vec3 normalPos;
} fs_in;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(fs_in.pos - viewPos);
    vec3 R = reflect(I, normalize(fs_in.normalPos));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 pos;
	vec2 texPos;
	vec3 normalPos;
} fs_in;

uniform sampler2D text;
uniform vec3 viewPos;

struct Material 
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    float shininess;
}; 
uniform Material material;

uniform vec4 globalAmbiantColor;
struct DirectionalLight 
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
    float intensity;

    vec3 direction;
};  
uniform DirectionalLight directionalLight;

struct PointLight  
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    vec3 position;
    float constant;
    float linear;
    float quadratic; 
    float intensity;
};  
#define POINT_LIGHT_COUNT 4  
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform int pointLightCount = 0;

struct SpotLight  
{
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;

    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic; 
    float intensity;

    float spotCosAngle;
	float spotCosSmoothAngle;
};
#define SPOT_LIGHT_COUNT 4  
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];
uniform int spotLightCount = 0;

vec3 ComputeDirectionalLightColor(DirectionalLight dLight)
{
    vec3 lightDirection = normalize(dLight.direction);
    vec4 ambientColor = dLight.ambientColor * material.ambientColor * texture(text, fs_in.texPos); 
    vec4 diffuseColor = dLight.diffuseColor * material.diffuseColor * texture(text, fs_in.texPos); 
    float NdotL = dot(fs_in.normalPos , -lightDirection); 
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - fs_in.pos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor;
    if(NdotL >= 0.001)
    {
        specularColor = dLight.specularColor * material.specularColor * texture(text, fs_in.texPos); 
        specularColor*= pow(max(dot(fs_in.normalPos, halfwayDir), 0.0), material.shininess);
    }
  
    vec4 color = ambientColor + diffuseColor + specularColor;

    return vec3(color.r, color.g, color.b) * dLight.intensity;
}; 

vec3 ComputePointLight(PointLight pLight)
{
    vec3 lightDiff = fs_in.pos + pLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    vec4 ambientColor = pLight.ambientColor * material.ambientColor * texture(text, fs_in.texPos);
    vec4 diffuseColor = pLight.diffuseColor * material.diffuseColor * texture(text, fs_in.texPos);
    float NdotL = dot(fs_in.normalPos, lightDirection);
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - fs_in.pos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);

    vec4 specularColor;
    if(NdotL >= 0.001)
    {
        specularColor = pLight.specularColor * material.specularColor * texture(text, fs_in.texPos); 
        specularColor*= pow(max(dot(fs_in.normalPos, halfwayDir), 0.0), material.shininess);
    }
  
    float attenuation = 1.0 / (pLight.constant + pLight.linear * lightDistance +  pLight.quadratic * (lightDistance * lightDistance));    

    vec4 color = ambientColor+diffuseColor + specularColor;
    color*=attenuation;
  
    return vec3(color.r, color.g, color.b)* pLight.intensity;
}

vec3 ComputeSpotLight(SpotLight sLight)
{
    vec3 lightDiff = fs_in.pos + sLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    float cosAngle    = dot(lightDirection, normalize(sLight.direction));
    vec4 ambientColor = sLight.ambientColor * material.ambientColor * texture(text, fs_in.texPos);

    if(cosAngle > sLight.spotCosAngle)
    {
        vec4 diffuseColor = sLight.diffuseColor * material.diffuseColor * texture(text, fs_in.texPos);
        vec4 specularColor;

        
        float NdotL = dot(fs_in.normalPos, lightDirection);
        diffuseColor*= max(NdotL,0);

        vec3 viewDir = normalize(viewPos - fs_in.pos);
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        if(NdotL >= 0.001)
        {
            specularColor = sLight.specularColor * material.specularColor *  texture(text, fs_in.texPos);
            specularColor*= pow(max(dot(fs_in.normalPos, halfwayDir), 0.0), material.shininess);
        }
  
        float attenuation = 1.0 / (sLight.constant + sLight.linear * lightDistance +  sLight.quadratic * (lightDistance * lightDistance));    
        float epsilon   = sLight.spotCosSmoothAngle - sLight.spotCosAngle;
        float intensity = smoothstep(0.0, 1.0, (cosAngle - sLight.spotCosAngle) / epsilon); 
        vec4 color = ambientColor+diffuseColor + specularColor;
        color*=attenuation * intensity;

        return vec3(color.r, color.g, color.b)* sLight.intensity;
    }
    else
        return vec3(0, 0, 0);
}

void main()
{
    vec4 ambientColor = globalAmbiantColor; 
    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);
    color += ComputeDirectionalLightColor(directionalLight);
    
    for(int i = 0; i < pointLightCount; i++)
        color += ComputePointLight(pointLights[i]);

    for(int i = 0; i < spotLightCount; i++)
        color += ComputeSpotLight(spotLights[i]);

	FragColor = vec4(color.r, color.g, color.b, 1.0);
}
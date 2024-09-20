#version 460 core
out vec4 FragColor;

in VS_OUT
{
	vec3 worldPos;
	vec3 normalPos;
} fs_in;

vec4 tempColor;

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
    vec4 ambientColor = dLight.ambientColor * material.ambientColor * tempColor; 
    vec4 diffuseColor = dLight.diffuseColor * material.diffuseColor * tempColor; 
    float NdotL = dot(fs_in.normalPos , -lightDirection); 
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - fs_in.worldPos);
    vec3 halfwayDir = normalize(-lightDirection + viewDir);

    vec4 specularColor;
    if(NdotL >= 0.001)
    {
        specularColor = dLight.specularColor * material.specularColor * tempColor; 
        specularColor*= pow(max(dot(fs_in.normalPos, halfwayDir), 0.0), material.shininess);
    }
  
    vec4 color = ambientColor + diffuseColor + specularColor;

    return vec3(color.r, color.g, color.b) * dLight.intensity;
}; 

vec3 ComputePointLight(PointLight pLight)
{
    vec3 lightDiff = fs_in.worldPos + pLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    vec4 ambientColor = pLight.ambientColor * material.ambientColor * tempColor;
    vec4 diffuseColor = pLight.diffuseColor * material.diffuseColor * tempColor;
    float NdotL = dot(fs_in.normalPos, lightDirection);
    diffuseColor*= max(NdotL,0);

    vec3 viewDir = normalize(viewPos - fs_in.worldPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);

    vec4 specularColor;
    if(NdotL >= 0.001)
    {
        specularColor = pLight.specularColor * material.specularColor * tempColor; 
        specularColor*= pow(max(dot(fs_in.normalPos, halfwayDir), 0.0), material.shininess);
    }
  
    float attenuation = 1.0 / (pLight.constant + pLight.linear * lightDistance +  pLight.quadratic * (lightDistance * lightDistance));    

    vec4 color = ambientColor+diffuseColor + specularColor;
    color*=attenuation;
  
    return vec3(color.r, color.g, color.b)* pLight.intensity;
}

vec3 ComputeSpotLight(SpotLight sLight)
{
    vec3 lightDiff = fs_in.worldPos + sLight.position;

    float lightDistance = length(lightDiff);
    vec3 lightDirection = lightDiff/lightDistance;

    float cosAngle    = dot(lightDirection, normalize(sLight.direction));
    vec4 ambientColor = sLight.ambientColor * material.ambientColor * tempColor;

    if(cosAngle > sLight.spotCosAngle)
    {
        vec4 diffuseColor = sLight.diffuseColor * material.diffuseColor * tempColor;
        vec4 specularColor;

        
        float NdotL = dot(fs_in.normalPos, lightDirection);
        diffuseColor*= max(NdotL,0);

        vec3 viewDir = normalize(viewPos - fs_in.worldPos);
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        if(NdotL >= 0.001)
        {
            specularColor = sLight.specularColor * material.specularColor *  tempColor;
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
	if (fs_in.worldPos.y > 22)
	{
		tempColor = vec4(1, 1, 1, 1);
	}
	else if (fs_in.worldPos.y > 8)
	{
		tempColor = vec4(0.78, 0.75, 0.72, 1);
	}
	else if (fs_in.worldPos.y > -12)
	{
		tempColor = vec4(1, 0.9, 0.6, 1);
	}
	else if (fs_in.worldPos.y > -15)
	{
		tempColor = vec4(0.07, 0.49, 0.07, 1);
	}
	else
	{
		tempColor = vec4(0, 0, 1, 1);
	}

    vec4 ambientColor = globalAmbiantColor; 
    vec3 color = vec3(ambientColor.r, ambientColor.g, ambientColor.b);
    color += ComputeDirectionalLightColor(directionalLight);
    
    for(int i = 0; i < pointLightCount; i++)
        color += ComputePointLight(pointLights[i]);

    for(int i = 0; i < spotLightCount; i++)
        color += ComputeSpotLight(spotLights[i]);

	FragColor = vec4(color.r, color.g, color.b, 1.0);
}
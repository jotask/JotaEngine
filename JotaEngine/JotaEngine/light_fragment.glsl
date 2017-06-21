#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirectionalLight directionLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform Material material;

out vec4 color;

vec3 calculateDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir);

void main()
{

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calculateDirectionLight(directionLight, norm, viewDir);

	for(int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pointLights[i], norm, FragPos, viewDir);
	}

	result += calculateSpotLight(spotLight, norm, FragPos, viewDir);

	color = vec4(result, 1.0f);
	
}


vec3 calculateDirectionLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize( -light.direction );

	float diff = max( dot( normal, lightDir ), 0.0f );

	vec3 reflectDir = reflect( -lightDir, normal);

	float spec = pow( max( dot( viewDir, reflectDir ), 0.0f ), material.shininess );

	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	return (ambient + diffuse + specular);

}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
	vec3 lightDir = normalize( light.position - fragPosition );

	float diff = max( dot( normal, lightDir ), 0.0f );

	vec3 reflectDir = reflect( -lightDir, normal);

	float spec = pow( max( dot( viewDir, reflectDir ), 0.0f ), material.shininess );

	// Attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDir)
{
	vec3 lightDir = normalize( light.position - fragPosition );

	float diff = max( dot( normal, lightDir ), 0.0f );

	vec3 reflectDir = reflect( -lightDir, normal);

	float spec = pow( max( dot( viewDir, reflectDir ), 0.0f ), material.shininess );

	// Attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot( lightDir, normalize( -light.direction ) );
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0f, 1.0f );

	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}
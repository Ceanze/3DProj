#version 330

struct PointLightData
{
	vec4 positionRadius;
	vec4 colorIntensity;
};

struct DirectionalLightData
{
	vec4 direction;
	vec4 colorIntensity;
};

#define MAX_LIGHTS 5
layout(std140) uniform Lights
{
  PointLightData pointLightData[MAX_LIGHTS];
  DirectionalLightData directionalLightData;
};
uniform int nrOfPointLights;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D kd_a_Texture;
uniform sampler2D ks_ns_Texture;

layout(location = 0) out vec4 finalDiffuse;
layout(location = 1) out vec4 finalSpecular;

in vec2 fragTextureCoord;
uniform vec3 camPos;

uniform mat4 shadowCamera;
uniform sampler2D shadowMap;

void main()
{
	vec4 kd_a = texture(kd_a_Texture, fragTextureCoord);
	vec3 ambient = vec3(kd_a.w, kd_a.w, kd_a.w);

	vec4 ks_ns = texture(ks_ns_Texture, fragTextureCoord);

	vec3 fragPos = texture(positionTexture, fragTextureCoord).xyz;
	vec3 fragNormal = texture(normalTexture, fragTextureCoord).xyz;
	bool isBackground = fragNormal.x == 0.0 && fragNormal.y == 0.0 && fragNormal.z == 0.0;
	fragNormal = normalize(fragNormal);
	vec3 specular = vec3(0.0, 0.0, 0.0);
	vec3 diffuseOut = vec3(0.0, 0.0, 0.0);
   
	for(int i = 0; i < nrOfPointLights; i++)
	{
		// Diffuse part
		float fragToLight = length(pointLightData[i].positionRadius.xyz - fragPos);
		vec3 lightDirection = normalize(pointLightData[i].positionRadius.xyz - fragPos);
		float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0);
		float lightFactor = max(pointLightData[i].colorIntensity.w * (1.0 - (fragToLight / pointLightData[i].positionRadius.w)), 0.0);
		diffuseOut += diffuseFactor*lightFactor*pointLightData[i].colorIntensity.xyz;

		// Specular part
		float s = ks_ns.w;
		vec3 r = reflect(fragNormal, lightDirection);
		vec3 v = normalize(camPos - fragPos);
		float specularFactor = pow(max(dot(r, v), 0.0), s+0.01);
		specular += diffuseFactor*specularFactor*lightFactor;
	}
 
	float visibility = 1.0;
	const mat4 biasMatrix = mat4(0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0);
	mat4 depthBias = biasMatrix*shadowCamera;
	vec4 shadowCoord = depthBias*vec4(fragPos, 1.0);
	if(shadowCoord.x >= 0.0 && shadowCoord.x <= 1.0 && shadowCoord.y >= 0.0 && shadowCoord.y <= 1.0 && shadowCoord.z >= 0.0 && shadowCoord.z <= 1.0)
		if(texture(shadowMap, shadowCoord.xy).x+0.01 < shadowCoord.z)
			visibility = 0.5;
	
	// Directonal Light
	// Diffuse part
	vec3 lightDirection = -directionalLightData.direction.xyz;
	float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0);
	diffuseOut += visibility*diffuseFactor*directionalLightData.colorIntensity.xyz;

	// Specular part
	float s = ks_ns.w;
	vec3 r = reflect(fragNormal, lightDirection);
	vec3 v = normalize(camPos - fragPos);
	float specularFactor = pow(max(dot(r, v), 0.0), s+0.01);
	specular += visibility*diffuseFactor*specularFactor;

	ambient = isBackground ? vec3(0.0) : ambient;
	finalDiffuse = vec4(ambient + diffuseOut*kd_a.xyz, 1.0);
	finalSpecular = vec4(specular*ks_ns.xyz, 1.0);
}
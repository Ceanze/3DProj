#version 330

struct PointLightData
{
	vec4 positionRadius;
	vec4 colorIntensity;
};

#define MAX_LIGHTS 5
layout(std140) uniform Lights
{
  PointLightData pointLightData[MAX_LIGHTS];
};

uniform int nrOfPointLights;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D kd_a_Texture;
uniform sampler2D ks_ns_Texture;

out vec4 finalColor;

in vec2 fragTextureCoord;
uniform vec3 camPos;

void main()
{
	vec4 kd_a = texture(kd_a_Texture, fragTextureCoord);
	vec3 ambient = vec3(kd_a.w, kd_a.w, kd_a.w);

	vec4 ks_ns = texture(ks_ns_Texture, fragTextureCoord);

	vec3 fragPos = texture(positionTexture, fragTextureCoord).xyz;
	vec3 fragNormal = normalize(texture(normalTexture, fragTextureCoord).xyz);
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

	vec3 finalDiffuse = diffuseOut*kd_a.xyz + ambient;
	vec3 finalSpecular = specular*ks_ns.xyz;

	vec3 materialColor = texture(albedoTexture, fragTextureCoord).xyz;

	finalColor = vec4(materialColor*(ambient + finalDiffuse + finalSpecular), 1.0);
}
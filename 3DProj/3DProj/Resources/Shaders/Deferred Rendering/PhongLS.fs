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

layout(location=0) out vec4 finalDiffuse;
layout(location=1) out vec4 finalSpecular;

in vec2 fragTextureCoord;
uniform vec3 camPos;

//const vec3 lightPos = vec3(2.0, 2.0, -5.0);
const vec3 ambient = vec3(0.2, 0.2, 0.2);

void main()
{
  vec3 fragPos = texture(positionTexture, fragTextureCoord).xyz;
  vec3 fragNormal = normalize(texture(normalTexture, fragTextureCoord).xyz);
  vec3 specular = vec3(0.0f, 0.0f, 0.0f);
  float diffuseOut = 0;

   
    for(int i = 0; i < nrOfPointLights; i++)
    {
      // Diffuse part
      float fragToLight = length(pointLightData[i].positionRadius.xyz - fragPos);
      vec3 lightDirection = normalize(pointLightData[i].positionRadius.xyz - fragPos);
      float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0);
      float lightFactor = max(pointLightData[i].colorIntensity.w * (1.0 - (fragToLight / pointLightData[i].positionRadius.w)), 0.0);
      diffuseOut += diffuseFactor * lightFactor;

      // Specular part
      vec3 specularColor = vec3(1.0, 1.0, 1.0);
      float s = 80.0;
      vec3 r = reflect(fragNormal, lightDirection);
      vec3 v = normalize(camPos - fragPos);
      float specularFactor = pow(max(dot(r, v), 0.0), s);
      specular += specularFactor*specularColor*0.7 *lightFactor;
    }

    finalDiffuse = vec4(diffuseOut + ambient, 1.0);
    finalSpecular = vec4(specular, 1.0);
}
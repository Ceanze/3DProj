#version 330

struct PointLight
{
  float intensity;
  float radius;
  vec3 position;
  vec3 color;
};

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

layout(location=0) out vec4 finalDiffuse;
layout(location=1) out vec4 finalSpecular;

in vec2 fragTextureCoord;
uniform vec3 camPos;

const vec3 lightPos = vec3(2.0, 2.0, -5.0);

void main()
{
    vec3 fragPos = texture(positionTexture, fragTextureCoord).xyz;
    vec3 fragNormal = texture(normalTexture, fragTextureCoord).xyz;

     // Diffuse part
    vec3 fragToLight = normalize(lightPos - fragPos);
    float diffuseFactor = max(dot(fragNormal, fragToLight), 0.0);

      // Specular part
    vec3 specularColor = vec3(1.0, 1.0, 1.0);
    float s = 80.0;
    vec3 r = reflect(fragNormal, fragToLight);
    vec3 v = normalize(camPos - fragPos);
    float specularFactor = pow(max(dot(r, v), 0.0), s);
    vec3 specular = specularFactor*specularColor;
    
    finalDiffuse = vec4(diffuseFactor, diffuseFactor, diffuseFactor, 1.0);
    finalSpecular = vec4(specular, 1.0);
}
#version 330

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;

layout(location=0) out vec4 finalAmbient;
layout(location=1) out vec4 finalDiffuse;
layout(location=2) out vec4 finalSpecular;

in vec2 fragTextureCoord;
in vec3 camPos;

const vec3 lightPos = vec3(0.0, 2.0, 5.0);
const vec3 ambientFactor = vec3(0.2, 0.2, 0.2);

void main()
{
    vec3 fragPos = texture(positionTexture, fragTextureCoord).xyz;

     // Diffuse part
    vec3 fragToLight = normalize(lightPos - fragPos);
    float diffuseFactor = max(dot(texture(normalTexture, fragTextureCoord).xyz, fragToLight), 0.0);

      // Specular part
    vec3 specularColor = vec3(1.0, 1.0, 1.0);
    float s = 80.0;
    vec3 r = reflect(texture(normalTexture, fragTextureCoord).xyz, fragToLight);
    vec3 v = normalize(camPos - fragPos);
    float specularFactor = pow(max(dot(r, v), 0.0), s);
    vec3 specular = specularFactor*specularColor;
    
    finalAmbient = vec4(ambientFactor, 1.0);
    finalDiffuse = vec4(texture(depthTexture, fragTextureCoord).xyz * diffuseFactor, 1.0);
    finalSpecular = vec4(specular * texture(albedoTexture, fragTextureCoord).xyz, 1.0);
}
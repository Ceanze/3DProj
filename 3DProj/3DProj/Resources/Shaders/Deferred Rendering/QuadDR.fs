#version 330

in vec2 fragTextureCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D albedoTexture;

out vec4 finalColor;

void main()
{
    vec3 diffuse = texture(diffuseTexture, fragTextureCoord).xyz;
    vec3 specular = texture(specularTexture, fragTextureCoord).xyz;
    vec3 materialColor = texture(albedoTexture, fragTextureCoord).xyz;

    finalColor = min(vec4(materialColor * diffuse + materialColor * specular, 1.0f), 1.0);
}
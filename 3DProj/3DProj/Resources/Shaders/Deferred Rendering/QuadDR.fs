#version 330

in vec2 fragTextureCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D albedoTexture;

out vec4 finalColor;

const vec3 ambient = vec3(0.2, 0.2, 0.2);

void main()
{
    float diffuse = texture(diffuseTexture, fragTextureCoord).x;
    float specular = texture(specularTexture, fragTextureCoord).x;
    vec3 materialColor = texture(albedoTexture, fragTextureCoord).xyz;

    finalColor = min(vec4(materialColor * ambient + materialColor * diffuse + materialColor * specular, 1.0f), 1);
}
#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTextureCoord; 

layout(location=0) out vec4 finalPosition;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalAlbedo;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;

void main()
{
    finalAlbedo = texture(albedoMap, fragTextureCoord);

	finalNormal = vec4(normalize(fragNormal), 1.0);

    finalPosition = vec4(fragPosition, 1.0);
}
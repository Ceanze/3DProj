#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec3 fragTangent;
in vec2 fragTextureCoord; 

layout(location=0) out vec4 finalPosition;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalAlbedo;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;

void main()
{
    finalAlbedo = texture(albedoMap, fragTextureCoord);

	vec3 normal = normalize(fragNormal);
	vec3 tangent = normalize(fragTangent);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal); // Tangent space to world space.
	normal = normalize((tbn*(texture(normalMap, fragTextureCoord).xyz*2.0 - 1.0)));
    finalNormal = vec4(normal, 1.0);

    finalPosition = vec4(fragPosition, 1.0);
}
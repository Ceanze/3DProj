#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec3 fragTangent;
in vec2 fragTextureCoord; 

layout(std140) uniform Material
{
	vec4 kd_a;
    vec4 ks_ns;
} material;

layout(location=0) out vec4 finalPosition;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalAlbedo;
layout(location=3) out vec4 finalKd_a;
layout(location=4) out vec4 finalKs_ns;

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

	finalKd_a = material.kd_a;
	finalKs_ns = material.ks_ns;
}